const API_BASE = 'http://127.0.0.1:5000/api';
let allBooks = [];

async function fetchBooks() {
    try {
        const res = await fetch(`${API_BASE}/books`);
        allBooks = await res.json();
        renderBooks(allBooks);
        fetchPopular();
    } catch (err) {
        console.error("Error fetching books:", err);
    }
}

async function fetchPopular() {
    try {
        const res = await fetch(`${API_BASE}/books/popular`);
        const popular = await res.json();
        renderPopular(popular);
    } catch (err) {
        console.error("Error fetching popular books:", err);
    }
}

function renderBooks(books) {
    const grid = document.getElementById('booksGrid');
    grid.innerHTML = books.map(book => `
        <div class="card">
            <span class="badge">${book.category}</span>
            <h3 class="card-title">${book.title}</h3>
            <p class="card-author">by ${book.author}</p>
            <p style="font-size: 0.8rem; color: var(--text-muted); margin-bottom: 0.5rem;">ISBN: ${book.isbn}</p>
            <div class="rating-stars">
                ${'★'.repeat(Math.round(getAverageRating(book.ratings)))}${'☆'.repeat(5 - Math.round(getAverageRating(book.ratings)))}
                <span style="font-size: 0.8rem; color: var(--text-muted);">(${book.ratings.length})</span>
            </div>
            <div class="stats">
                <span>Available: <b>${book.copies}</b></span>
                <span>Issued: <b>${book.issuedCount}</b></span>
            </div>
            <div class="actions">
                <button class="btn btn-small" onclick="issueBook('${book.isbn}')" style="background: #3b82f6;">Issue</button>
                <button class="btn btn-small" onclick="returnBook('${book.isbn}')" style="background: #10b981;">Return</button>
                <button class="btn btn-small" onclick="promptRate('${book.isbn}')" style="background: #f59e0b;">Rate</button>
                <button class="btn btn-small" onclick="openUpdateModal('${book.isbn}')" style="background: #6366f1;">Edit</button>
                <button class="btn btn-small" onclick="deleteBook('${book.isbn}')" style="background: #ef4444; grid-column: span 2;">Delete</button>
            </div>
        </div>
    `).join('');
}

function renderPopular(books) {
    const grid = document.getElementById('popularGrid');
    grid.innerHTML = books.map(book => `
        <div class="card" style="border-color: rgba(245, 158, 11, 0.4);">
            <h4 class="card-title">${book.title}</h4>
            <p class="card-author" style="margin-bottom: 0.5rem;">${book.issuedCount} issues</p>
            <div class="rating-stars">
                 ${'★'.repeat(Math.round(getAverageRating(book.ratings)))}
            </div>
        </div>
    `).join('');
}

function getAverageRating(ratings) {
    if (!ratings || ratings.length === 0) return 0;
    return ratings.reduce((a, b) => a + b, 0) / ratings.length;
}

// Actions
async function issueBook(isbn) {
    const res = await fetch(`${API_BASE}/books/${isbn}/issue`, { method: 'POST' });
    if (res.ok) fetchBooks();
    else alert("Cannot issue book.");
}

async function returnBook(isbn) {
    const res = await fetch(`${API_BASE}/books/${isbn}/return`, { method: 'POST' });
    if (res.ok) fetchBooks();
}

async function promptRate(isbn) {
    const rating = prompt("Enter rating (1-5):");
    if (rating >= 1 && rating <= 5) {
        await fetch(`${API_BASE}/books/${isbn}/rate`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ rating: parseInt(rating) })
        });
        fetchBooks();
    }
}

async function deleteBook(isbn) {
    const pass = prompt("Enter admin password to delete:");
    if (!pass) return;
    const res = await fetch(`${API_BASE}/books/${isbn}`, {
        method: 'DELETE',
        headers: { 'Authorization': pass }
    });
    if (res.ok) fetchBooks();
    else alert("Access Denied or book not found.");
}

function filterBooks() {
    const query = document.getElementById('searchInput').value.toLowerCase();
    const filtered = allBooks.filter(b => 
        b.title.toLowerCase().includes(query) || 
        b.author.toLowerCase().includes(query) || 
        b.isbn.includes(query)
    );
    renderBooks(filtered);
}

// Modal logic
function openModal(id) {
    document.getElementById(id).style.display = 'flex';
}

function closeModal(id) {
    document.getElementById(id).style.display = 'none';
}

function openUpdateModal(isbn) {
    const book = allBooks.find(b => b.isbn === isbn);
    document.getElementById('updateIsbn').value = isbn;
    document.getElementById('uTitle').value = book.title;
    document.getElementById('uAuthor').value = book.author;
    document.getElementById('uCategory').value = book.category;
    document.getElementById('uCopies').value = book.copies;
    openModal('updateBookModal');
}

document.getElementById('addBookForm').onsubmit = async (e) => {
    e.preventDefault();
    const book = {
        title: document.getElementById('title').value,
        author: document.getElementById('author').value,
        isbn: document.getElementById('isbn').value,
        category: document.getElementById('category').value,
        copies: parseInt(document.getElementById('copies').value)
    };
    const res = await fetch(`${API_BASE}/books`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(book)
    });
    if (res.ok) {
        closeModal('addBookModal');
        fetchBooks();
        e.target.reset();
    } else {
        const err = await res.json();
        alert(err.error);
    }
};

document.getElementById('updateBookForm').onsubmit = async (e) => {
    e.preventDefault();
    const isbn = document.getElementById('updateIsbn').value;
    const pass = document.getElementById('adminPass').value;
    const book = {
        title: document.getElementById('uTitle').value,
        author: document.getElementById('uAuthor').value,
        category: document.getElementById('uCategory').value,
        copies: parseInt(document.getElementById('uCopies').value)
    };
    const res = await fetch(`${API_BASE}/books/${isbn}`, {
        method: 'PUT',
        headers: { 
            'Content-Type': 'application/json',
            'Authorization': pass
        },
        body: JSON.stringify(book)
    });
    if (res.ok) {
        closeModal('updateBookModal');
        fetchBooks();
    } else {
        alert("Access Denied or error updating.");
    }
};

// Initial load
fetchBooks();

#include "Library.h"
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

void Library::addBook() {
    string title, author, isbn, category;
    int copies;
    cout << "Enter title: "; cin.ignore(); getline(cin, title);
    cout << "Enter author: "; getline(cin, author);
    cout << "Enter ISBN: "; getline(cin, isbn);
    cout << "Enter category: "; getline(cin, category);
    cout << "Enter number of copies: "; cin >> copies;
    books.push_back(Book(title, author, isbn, category, copies));
    cout << "Book added successfully!\n";
}

void Library::updateBook() {
    string pass;
    cout << "Enter admin password: "; cin >> pass;
    if (pass != adminPassword) { cout << "Access Denied!\n"; return; }

    string isbn;
    cout << "Enter ISBN of book to update: "; cin.ignore(); getline(cin, isbn);
    for (auto &b : books) {
        if (b.getISBN() == isbn) {
            string title, author, newIsbn, category;
            int copies;
            cout << "Enter new title: "; getline(cin, title);
            cout << "Enter new author: "; getline(cin, author);
            cout << "Enter new ISBN: "; getline(cin, newIsbn);
            cout << "Enter category: "; getline(cin, category);
            cout << "Enter copies: "; cin >> copies;
            b.updateBook(title, author, newIsbn, category, copies);
            cout << "Book updated successfully.\n";
            return;
        }
    }
    cout << "Book not found!\n";
}

void Library::deleteBook() {
    string pass;
    cout << "Enter admin password: "; cin >> pass;
    if (pass != adminPassword) { cout << "Access Denied!\n"; return; }

    string isbn;
    cout << "Enter ISBN of book to delete: "; cin.ignore(); getline(cin, isbn);
    for (auto it = books.begin(); it != books.end(); ++it) {
        if (it->getISBN() == isbn) {
            books.erase(it);
            cout << "Book deleted successfully.\n";
            return;
        }
    }
    cout << "Book not found!\n";
}

void Library::issueBook() {
    string isbn;
    cout << "Enter ISBN to issue: "; cin.ignore(); getline(cin, isbn);
    for (auto &b : books) {
        if (b.getISBN() == isbn) { b.issueBook(); return; }
    }
    cout << "Book not found!\n";
}

void Library::returnBook() {
    string isbn;
    cout << "Enter ISBN to return: "; cin.ignore(); getline(cin, isbn);
    for (auto &b : books) {
        if (b.getISBN() == isbn) { b.returnBook(); return; }
    }
    cout << "Book not found!\n";
}

void Library::rateBook() {
    string isbn;
    int rating;
    cout << "Enter ISBN of book to rate: "; cin.ignore(); getline(cin, isbn);
    cout << "Enter rating (1-5): "; cin >> rating;
    for (auto &b : books) {
        if (b.getISBN() == isbn) {
            b.addRating(rating);
            cout << "Thank you! Rating added.\n";
            return;
        }
    }
    cout << "Book not found!\n";
}

void Library::filterByCategory() const {
    string category;
    cout << "Enter category to filter: "; cin.ignore(); getline(cin, category);
    bool found = false;
    for (auto &b : books) {
        if (b.getCategory() == category) { b.display(); found = true; }
    }
    if (!found) cout << "No books in this category.\n";
}

void Library::showPopularBooks() const {
    vector<Book> sorted = books;
    sort(sorted.begin(), sorted.end(), [](Book a, Book b) {
        return a.getIssuedCount() > b.getIssuedCount();
    });
    cout << "--- Most Popular Books ---\n";
    for (int i = 0; i < 3 && i < (int)sorted.size(); i++) {
        sorted[i].display();
    }
}

void Library::displayAllBooks() const {
    if (books.empty()) { cout << "No books available.\n"; return; }
    for (auto &b : books) b.display();
}

void Library::saveToFile() const {
    ofstream fout("library_data.txt");
    for (auto &b : books) fout << b.toString() << "\n";
    fout.close();
    cout << "Library data saved.\n";
}

void Library::loadFromFile() {
    ifstream fin("library_data.txt");
    if (!fin) return;
    string line;
    while (getline(fin, line)) {
        books.push_back(Book::fromString(line));
    }
    fin.close();
}

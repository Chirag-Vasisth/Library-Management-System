from flask import Flask, request, jsonify
from flask_cors import CORS
import json
import os

app = Flask(__name__)
CORS(app)

DATA_FILE = 'data.json'
ADMIN_PASSWORD = 'admin123'

def load_data():
    if os.path.exists(DATA_FILE):
        with open(DATA_FILE, 'r') as f:
            return json.load(f)
    return []

def save_data(data):
    with open(DATA_FILE, 'w') as f:
        json.dump(data, f, indent=4)

@app.route('/api/books', methods=['GET'])
def get_books():
    return jsonify(load_data())

@app.route('/api/books', methods=['POST'])
def add_book():
    data = load_data()
    new_book = request.json
    # Basic validation
    if not new_book.get('isbn'):
        return jsonify({"error": "ISBN is required"}), 400
    
    # Check if book already exists
    for book in data:
        if book['isbn'] == new_book['isbn']:
            return jsonify({"error": "Book with this ISBN already exists"}), 400
            
    new_book['issuedCount'] = 0
    new_book['ratings'] = []
    data.append(new_book)
    save_data(data)
    return jsonify(new_book), 201

@app.route('/api/books/<isbn>', methods=['PUT'])
def update_book(isbn):
    auth_password = request.headers.get('Authorization')
    if auth_password != ADMIN_PASSWORD:
        return jsonify({"error": "Access Denied!"}), 403

    data = load_data()
    updated_book = request.json
    for i, book in enumerate(data):
        if book['isbn'] == isbn:
            data[i].update(updated_book)
            save_data(data)
            return jsonify(data[i])
    return jsonify({"error": "Book not found"}), 404

@app.route('/api/books/<isbn>', methods=['DELETE'])
def delete_book(isbn):
    auth_password = request.headers.get('Authorization')
    if auth_password != ADMIN_PASSWORD:
        return jsonify({"error": "Access Denied!"}), 403

    data = load_data()
    for i, book in enumerate(data):
        if book['isbn'] == isbn:
            del data[i]
            save_data(data)
            return jsonify({"message": "Book deleted successfully"})
    return jsonify({"error": "Book not found"}), 404

@app.route('/api/books/<isbn>/issue', methods=['POST'])
def issue_book(isbn):
    data = load_data()
    for book in data:
        if book['isbn'] == isbn:
            if book['copies'] > 0:
                book['copies'] -= 1
                book['issuedCount'] += 1
                save_data(data)
                return jsonify(book)
            else:
                return jsonify({"error": "No copies available"}), 400
    return jsonify({"error": "Book not found"}), 404

@app.route('/api/books/<isbn>/return', methods=['POST'])
def return_book(isbn):
    data = load_data()
    for book in data:
        if book['isbn'] == isbn:
            book['copies'] += 1
            save_data(data)
            return jsonify(book)
    return jsonify({"error": "Book not found"}), 404

@app.route('/api/books/<isbn>/rate', methods=['POST'])
def rate_book(isbn):
    rating = request.json.get('rating')
    if not (1 <= rating <= 5):
        return jsonify({"error": "Rating must be between 1 and 5"}), 400

    data = load_data()
    for book in data:
        if book['isbn'] == isbn:
            book['ratings'].append(rating)
            save_data(data)
            return jsonify(book)
    return jsonify({"error": "Book not found"}), 404

@app.route('/api/books/popular', methods=['GET'])
def get_popular_books():
    data = load_data()
    sorted_books = sorted(data, key=lambda x: x['issuedCount'], reverse=True)
    return jsonify(sorted_books[:3])

if __name__ == '__main__':
    app.run(debug=True, port=5000)

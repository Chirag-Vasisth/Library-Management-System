#include "Book.h"
#include <iostream>
#include <sstream>
using namespace std;

Book::Book(string t, string a, string i, string c, int copy)
    : title(t), author(a), isbn(i), category(c), copies(copy), issuedCount(0) {}

string Book::getISBN() const { return isbn; }
string Book::getCategory() const { return category; }
int Book::getIssuedCount() const { return issuedCount; }

void Book::display() const {
    cout << "Title: " << title << "\nAuthor: " << author
         << "\nISBN: " << isbn << "\nCategory: " << category
         << "\nCopies: " << copies << "\nIssued Count: " << issuedCount
         << "\nAverage Rating: " << getAverageRating() << "\n\n";
}

void Book::updateBook(string t, string a, string i, string c, int copy) {
    title = t; author = a; isbn = i; category = c; copies = copy;
}

void Book::issueBook() {
    if (copies > 0) {
        copies--;
        issuedCount++;
        cout << "Book issued successfully!\n";
    } else {
        cout << "No copies available.\n";
    }
}

void Book::returnBook() {
    copies++;
    cout << "Book returned successfully!\n";
}

void Book::addRating(int r) {
    if (r >= 1 && r <= 5) ratings.push_back(r);
}

double Book::getAverageRating() const {
    if (ratings.empty()) return 0.0;
    double sum = 0;
    for (int r : ratings) sum += r;
    return sum / ratings.size();
}

string Book::toString() const {
    stringstream ss;
    ss << title << "|" << author << "|" << isbn << "|" << category 
       << "|" << copies << "|" << issuedCount;
    return ss.str();
}

Book Book::fromString(string line) {
    stringstream ss(line);
    string t, a, i, c;
    int copy, issued;
    getline(ss, t, '|');
    getline(ss, a, '|');
    getline(ss, i, '|');
    getline(ss, c, '|');
    ss >> copy;
    ss.ignore();
    ss >> issued;

    Book b(t, a, i, c, copy);
    b.issuedCount = issued;
    return b;
}

#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>
using namespace std;

class Book {
private:
    string title, author, isbn, category;
    int copies;
    int issuedCount;
    vector<int> ratings;

public:
    Book(string t, string a, string i, string c, int copy);

    string getISBN() const;
    string getCategory() const;
    int getIssuedCount() const;

    void display() const;
    void updateBook(string t, string a, string i, string c, int copy);
    void issueBook();
    void returnBook();
    void addRating(int r);
    double getAverageRating() const;

    string toString() const; // For file save
    static Book fromString(string line); // For file load
};

#endif

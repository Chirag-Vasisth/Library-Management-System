#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include <vector>
#include <string>
using namespace std;

class Library {
private:
    vector<Book> books;
    string adminPassword = "admin123";

public:
    void addBook();
    void updateBook();
    void deleteBook();
    void issueBook();
    void returnBook();
    void rateBook();
    void filterByCategory() const;
    void showPopularBooks() const;
    void displayAllBooks() const;

    void saveToFile() const;
    void loadFromFile();
};

#endif

#include "Library.h"
#include <iostream>
using namespace std;

int main() {
    Library lib;
    lib.loadFromFile();
    int choice;

    do {
        cout << "\n--- Library Menu ---\n";
        cout << "1. Add Book\n";
        cout << "2. Update Book\n";
        cout << "3. Delete Book\n";
        cout << "4. Issue Book\n";
        cout << "5. Return Book\n";
        cout << "6. Rate Book\n";
        cout << "7. Filter by Category\n";
        cout << "8. Show Popular Books\n";
        cout << "9. Display All Books\n";
        cout << "10. Save & Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: lib.addBook(); break;
            case 2: lib.updateBook(); break;
            case 3: lib.deleteBook(); break;
            case 4: lib.issueBook(); break;
            case 5: lib.returnBook(); break;
            case 6: lib.rateBook(); break;
            case 7: lib.filterByCategory(); break;
            case 8: lib.showPopularBooks(); break;
            case 9: lib.displayAllBooks(); break;
            case 10: lib.saveToFile(); cout << "Exiting...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 10);

    return 0;
}

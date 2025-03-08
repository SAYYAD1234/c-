#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

class Book {
public:
    string ISBN, title, author;
    int publicationYear;
    bool isAvailable;

    Book(string isbn, string t, string a, int year)
        : ISBN(isbn), title(t), author(a), publicationYear(year), isAvailable(true) {}
};

class Library {
private:
    vector<Book> books;

public:
    void addBook() {
        string isbn, title, author;
        int year;

        cout << "\nEnter Book Details:\n";
        cout << "ISBN: "; cin >> isbn;
        cin.ignore();
        cout << "Title: "; getline(cin, title);
        cout << "Author: "; getline(cin, author);
        cout << "Publication Year: "; cin >> year;

        books.emplace_back(isbn, title, author, year);
        cout << "Book added successfully!\n";
    }

    void displayBooks() {
        cout << "\nLibrary Catalog:\n";
        cout << left << setw(15) << "ISBN" << setw(25) << "Title" 
             << setw(20) << "Author" << setw(10) << "Year" << "Status\n";
        for (const auto& book : books) {
            cout << left << setw(15) << book.ISBN << setw(25) << book.title 
                 << setw(20) << book.author << setw(10) << book.publicationYear
                 << (book.isAvailable ? "Available" : "Checked Out") << endl;
        }
    }

    Book* searchByISBN(const string& isbn) {
        for (auto& book : books) {
            if (book.ISBN == isbn) return &book;
        }
        return nullptr;
    }

    void checkoutBook() {
        string isbn;
        cout << "Enter ISBN of book to checkout: "; cin >> isbn;
        Book* book = searchByISBN(isbn);
        if (book) book->isAvailable ? (book->isAvailable = false, cout << "Book checked out successfully!\n") : cout << "Book is already checked out!\n";
        else cout << "Book not found!\n";
    }

    void returnBook() {
        string isbn;
        cout << "Enter ISBN of book to return: "; cin >> isbn;
        Book* book = searchByISBN(isbn);
        if (book) !book->isAvailable ? (book->isAvailable = true, cout << "Book returned successfully!\n") : cout << "Book was not checked out!\n";
        else cout << "Book not found!\n";
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        if (!file) { cout << "Error opening file!\n"; return; }
        for (const auto& book : books)
            file << book.ISBN << "," << book.title << "," << book.author << "," << book.publicationYear << "," << book.isAvailable << "\n";
        cout << "Data saved successfully!\n";
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) { cout << "File not found. Starting fresh.\n"; return; }
        books.clear();
        string line, isbn, title, author, temp;
        int year;
        bool available;
        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, isbn, ','); getline(ss, title, ','); getline(ss, author, ',');
            getline(ss, temp, ','); year = stoi(temp);
            getline(ss, temp); available = (temp == "1");
            books.emplace_back(isbn, title, author, year);
            books.back().isAvailable = available;
        }
        cout << "Data loaded successfully!\n";
    }
};

void displayMenu() {
    cout << "\nLibrary Management System\n";
    cout << "1. Add Book\n2. Display Books\n3. Search Book\n4. Checkout Book\n5. Return Book\n6. Save Data\n7. Load Data\n8. Exit\nEnter choice: ";
}

int main() {
    Library library;
    string filename = "library_data.txt";
    int choice;

    library.loadFromFile(filename);

    do {
        displayMenu();
        cin >> choice;
        switch (choice) {
            case 1: library.addBook(); break;
            case 2: library.displayBooks(); break;
            case 3: {
                string isbn;
                cout << "Enter ISBN to search: "; cin >> isbn;
                Book* book = library.searchByISBN(isbn);
                book ? (cout << "Title: " << book->title << "\nAuthor: " << book->author << "\nYear: " << book->publicationYear << endl) : cout << "Book not found!\n";
                break;
            }
            case 4: library.checkoutBook(); break;
            case 5: library.returnBook(); break;
            case 6: library.saveToFile(filename); break;
            case 7: library.loadFromFile(filename); break;
            case 8: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 8);

    return 0;
}

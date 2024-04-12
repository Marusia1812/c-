#ifndef BOOKDAO_H
#define BOOKDAO_H

#include "Book.h"
#include <memory>
#include "DBConnection.h"

class BookDAO {
public:
    static std::string addBook(const Book& book);
    static std::unique_ptr<Book> getBookByISBN(const std::string& isbn);
    static void updateBook(const Book& book);
    static void deleteBook(const std::string& isbn);
    static std::vector<Book> getAll();
    static std::vector<Book> getAllAvailable();
};

#endif

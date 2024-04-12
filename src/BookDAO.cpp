#include "BookDAO.h"
#include <iostream>

std::string BookDAO::addBook(const Book& book) {
    try {
        auto& session = DBConnection::getInstance()->getSession();
        auto table = session.getSchema(DBConnection::defaultSchemaName).getTable("Book");
        table.insert("isbn", "published_year", "title", "genre_id", "library_id", "status")
            .values(book.isbn, book.publishedYear, book.title, book.genreId, book.libraryId, book.status)
            .execute();

        return book.isbn;
    }
    catch (const std::exception& e) {
        return "";
    }
}

std::unique_ptr<Book> BookDAO::getBookByISBN(const std::string& isbn) {
    try {
        auto& session = DBConnection::getInstance()->getSession();
        auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("Book")
            .select("isbn", "published_year", "title", "genre_id", "library_id", "status")
            .where("isbn = :isbn")
            .bind("isbn", isbn)
            .execute();
        auto row = result.fetchOne();
        if (!row.isNull()) {
            return std::make_unique<Book>(row[0].get<std::string>(), row[1].get<int>(), row[2].get<std::string>(),
                row[3].get<std::string>(), row[4].get<int>(), row[5].get<std::string>());
        }
        return nullptr;
    }
    catch (const std::exception& e) {
        return nullptr;
    }
}

void BookDAO::updateBook(const Book& book) {
    auto& session = DBConnection::getInstance()->getSession();
    session.getSchema(DBConnection::defaultSchemaName).getTable("Book")
        .update()
        .set("published_year", book.publishedYear)
        .set("title", book.title)
        .set("genre_id", book.genreId)
        .set("library_id", book.libraryId)
        .set("status", book.status)
        .where("isbn = :isbn")
        .bind("isbn", book.isbn)
        .execute();
}

void BookDAO::deleteBook(const std::string& isbn) {
    auto& session = DBConnection::getInstance()->getSession();
    session.getSchema(DBConnection::defaultSchemaName).getTable("Book")
        .remove()
        .where("isbn = :isbn")
        .bind("isbn", isbn)
        .execute();
}

std::vector<Book> BookDAO::getAll() {
    std::vector<Book> books;
    auto& session = DBConnection::getInstance()->getSession();
    auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("Book")
        .select("isbn", "published_year", "title", "genre_id", "library_id", "status", "person_id")
        .execute();

    for (auto row : result) {
        uint32_t personId = 0;
        try {
            personId = row[6].get<uint32_t>();
        }
        catch (...) {}

        books.emplace_back(row[0].get<std::string>(), row[1].get<int>(), row[2].get<std::string>(),
            row[3].get<std::string>(), row[4].get<int>(), row[5].get<std::string>(), personId);
    }

    return books;
}

std::vector<Book> BookDAO::getAllAvailable() {
    std::vector<Book> books;
    auto& session = DBConnection::getInstance()->getSession();
    auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("Book")
        .select("isbn", "published_year", "title", "genre_id", "library_id", "status", "person_id")
        .where("status = :status")
        .bind("status", "Available")
        .execute();

    for (auto row : result) {
        books.emplace_back(row[0].get<std::string>(), row[1].get<int>(), row[2].get<std::string>(),
            row[3].get<std::string>(), row[4].get<int>(), row[5].get<std::string>());
    }

    return books;
}
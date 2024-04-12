#include "BookAuthorDAO.h"

void BookAuthorDAO::addBookAuthor(const std::string& isbn, uint32_t authorId) {
    auto& session = DBConnection::getInstance()->getSession();
    auto table = session.getSchema(DBConnection::defaultSchemaName).getTable("BookAuthor");
    table.insert("isbn", "author_id")
        .values(isbn, authorId)
        .execute();
}

void BookAuthorDAO::deleteBookAuthor(const std::string& isbn, uint32_t authorId) {
    auto& session = DBConnection::getInstance()->getSession();
    session.getSchema(DBConnection::defaultSchemaName).getTable("BookAuthor")
        .remove()
        .where("isbn = :isbn AND author_id = :authorId")
        .bind("isbn", isbn)
        .bind("authorId", authorId)
        .execute();
}

std::vector<uint32_t> BookAuthorDAO::getAuthorsForBook(const std::string& isbn) {
    auto& session = DBConnection::getInstance()->getSession();
    auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("BookAuthor")
        .select("author_id")
        .where("isbn = :isbn")
        .bind("isbn", isbn)
        .execute();

    std::vector<uint32_t> authorIds;
    for (auto row : result) {
        authorIds.push_back(row[0].get<uint32_t>());
    }
    return authorIds;
}

std::vector<std::string> BookAuthorDAO::getBooksForAuthor(uint32_t authorId) {
    auto& session = DBConnection::getInstance()->getSession();
    auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("BookAuthor")
        .select("isbn")
        .where("author_id = :authorId")
        .bind("authorId", authorId)
        .execute();

    std::vector<std::string> isbns;
    for (auto row : result) {
        isbns.push_back(row[0].get<std::string>());
    }
    return isbns;
}

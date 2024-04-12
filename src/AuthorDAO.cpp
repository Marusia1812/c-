#include "AuthorDAO.h"

uint32_t AuthorDAO::addAuthor(const Author& author) {
    auto& session = DBConnection::getInstance()->getSession();
    auto table = session.getSchema(DBConnection::defaultSchemaName).getTable("Author");
    return table.insert("fullname")
        .values(author.fullname)
        .execute()
        .getAutoIncrementValue();
}

std::unique_ptr<Author> AuthorDAO::getAuthorById(uint32_t id) {
    auto& session = DBConnection::getInstance()->getSession();
    auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("Author")
        .select("id", "fullname")
        .where("id = :id")
        .bind("id", id)
        .execute();
    auto row = result.fetchOne();
    if (!row.isNull()) {
        return std::make_unique<Author>(row[0].get<uint32_t>(), row[1].get<std::string>());
    }
    return nullptr;
}

void AuthorDAO::updateAuthor(const Author& author) {
    auto& session = DBConnection::getInstance()->getSession();
    session.getSchema(DBConnection::defaultSchemaName).getTable("Author")
        .update()
        .set("fullname", author.fullname)
        .where("id = :id")
        .bind("id", author.id)
        .execute();
}

void AuthorDAO::deleteAuthor(uint32_t id) {
    auto& session = DBConnection::getInstance()->getSession();
    session.getSchema(DBConnection::defaultSchemaName).getTable("Author")
        .remove()
        .where("id = :id")
        .bind("id", id)
        .execute();
}

std::unique_ptr<Author> AuthorDAO::getAuthorByName(std::string& name) {
    auto& session = DBConnection::getInstance()->getSession();
    auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("Author")
        .select("id", "fullname")
        .where("fullname = :name")
        .bind("name", name)
        .execute();
    auto row = result.fetchOne();
    if (!row.isNull()) {
        return std::make_unique<Author>(row[0].get<uint32_t>(), row[1].get<std::string>());
    }
    return nullptr;
}
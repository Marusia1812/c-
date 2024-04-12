#include "GenreDAO.h"

std::string GenreDAO::addGenre(const Genre& genre) {
    auto& session = DBConnection::getInstance()->getSession();
    auto table = session.getSchema(DBConnection::defaultSchemaName).getTable("Genre");
    table.insert("name")
        .values(genre.name)
        .execute();

    return genre.name;
}

std::unique_ptr<Genre> GenreDAO::getGenreByName(const std::string& name) {
    auto& session = DBConnection::getInstance()->getSession();
    auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("Genre")
        .select("name")
        .where("name = :name")
        .bind("name", name)
        .execute();
    auto row = result.fetchOne();
    if (!row.isNull()) {
        return std::make_unique<Genre>(row[0].get<std::string>());
    }
    return nullptr;
}

void GenreDAO::updateGenre(const std::string& oldName, const std::string& newName) {
    auto& session = DBConnection::getInstance()->getSession();
    session.getSchema(DBConnection::defaultSchemaName).getTable("Genre")
        .update()
        .set("name", newName)
        .where("name = :name")
        .bind("name", oldName)
        .execute();
}

void GenreDAO::deleteGenre(const std::string& name) {
    auto& session = DBConnection::getInstance()->getSession();
    session.getSchema(DBConnection::defaultSchemaName).getTable("Genre")
        .remove()
        .where("name = :name")
        .bind("name", name)
        .execute();
}

std::vector<Genre> GenreDAO::getAll() {
    std::vector<Genre> genres;
    auto& session = DBConnection::getInstance()->getSession();
    auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("Genre")
        .select("name")
        .execute();

    for (auto row : result) {
        genres.emplace_back(row[0].get<std::string>());
    }

    return genres;
}

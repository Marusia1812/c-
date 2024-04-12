#include "LibraryDAO.h"

uint32_t LibraryDAO::addLibrary(const Library& library) {
    auto& session = DBConnection::getInstance()->getSession();
    auto table = session.getSchema(DBConnection::defaultSchemaName).getTable("Library");
    return table.insert("name")
        .values(library.name)
        .execute()
        .getAutoIncrementValue();
}

std::unique_ptr<Library> LibraryDAO::getLibraryById(uint32_t id) {
    auto& session = DBConnection::getInstance()->getSession();
    auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("Library")
        .select("id", "name")
        .where("id = :id")
        .bind("id", id)
        .execute();
    auto row = result.fetchOne();
    if (!row.isNull()) {
        return std::make_unique<Library>(row[0].get<uint32_t>(), row[1].get<std::string>());
    }
    return nullptr;
}

void LibraryDAO::updateLibrary(const Library& library) {
    auto& session = DBConnection::getInstance()->getSession();
    session.getSchema(DBConnection::defaultSchemaName).getTable("Library")
        .update()
        .set("name", library.name)
        .where("id = :id")
        .bind("id", library.id)
        .execute();
}

void LibraryDAO::deleteLibrary(uint32_t id) {
    auto& session = DBConnection::getInstance()->getSession();
    session.getSchema(DBConnection::defaultSchemaName).getTable("Library")
        .remove()
        .where("id = :id")
        .bind("id", id)
        .execute();
}

std::vector<Library> LibraryDAO::getAll() {
    std::vector<Library> librares;
    auto& session = DBConnection::getInstance()->getSession();
    auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("Library")
        .select("id", "name")
        .execute();

    for (auto row : result) {
        librares.emplace_back(row[0].get<uint32_t>(), row[1].get<std::string>());
    }

    return librares;
}

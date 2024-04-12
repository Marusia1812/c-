#include "PersonDAO.h"
#include "DBConnection.h"
#include <iostream>

uint64_t PersonDAO::addPerson(const Person& person) {
    auto& session = DBConnection::getInstance()->getSession();
    auto table = session.getSchema(DBConnection::defaultSchemaName).getTable("Person");
    return table.insert("fullname", "role", "username", "password")
        .values(person.fullname, person.role, person.username, person.password)
        .execute()
        .getAutoIncrementValue();
}

std::unique_ptr<Person> PersonDAO::getPersonById(uint64_t id) {
    auto& session = DBConnection::getInstance()->getSession();
    auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("Person")
        .select("id", "fullname", "role", "username", "password")
        .where("id = :id")
        .bind("id", id)
        .execute();
    auto row = result.fetchOne();
    if (!row.isNull()) {
        return std::make_unique<Person>(row[0], row[1].get<std::string>(), row[2].get<std::string>(),
            row[3].get<std::string>(), row[4].get<std::string>());
    }
    return nullptr;
}

std::vector<Person> PersonDAO::getAll() {
    std::vector<Person> persons;
    auto& session = DBConnection::getInstance()->getSession();
    auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("Person")
        .select("id", "fullname", "role", "username")
        .execute();

    for (auto row : result) {
        persons.emplace_back(row[0].get<uint32_t>(), row[1].get<std::string>(),
            row[2].get<std::string>(), row[3].get<std::string>(), "");
    }

    return persons;
}

std::unique_ptr<Person> PersonDAO::getPersonByUsername(const std::string& username) {
    auto& session = DBConnection::getInstance()->getSession();
    auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("Person")
        .select("id", "fullname", "role", "username", "password")
        .where("username = :username")
        .bind("username", username)
        .execute();
    auto row = result.fetchOne();
    if (!row.isNull()) {
        return std::make_unique<Person>(row[0], row[1].get<std::string>(), row[2].get<std::string>(),
            row[3].get<std::string>(), row[4].get<std::string>());
    }
    return nullptr;
}


void PersonDAO::updatePerson(const Person& person) {
    auto& session = DBConnection::getInstance()->getSession();
    session.getSchema(DBConnection::defaultSchemaName).getTable("Person")
        .update()
        .set("fullname", person.fullname)
        .set("role", person.role)
        .set("username", person.username)
        .set("password", person.password)
        .where("id = :id")
        .bind("id", person.id)
        .execute();
}

bool PersonDAO::deletePerson(uint64_t id) {
    try {
        auto& session = DBConnection::getInstance()->getSession();
        session.getSchema(DBConnection::defaultSchemaName).getTable("Person")
            .remove()
            .where("id = :id")
            .bind("id", id)
            .execute();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return false;
    }

    return true;
}

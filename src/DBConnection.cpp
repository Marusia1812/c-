#include "DBConnection.h"

DBConnection* DBConnection::instance = nullptr;
const std::string DBConnection::defaultSchemaName = "library";

DBConnection::DBConnection(const std::string& host, unsigned int port, const std::string& user, const std::string& password)
    : session(host, port, user, password) {}

DBConnection* DBConnection::getInstance(const std::string& host, unsigned int port, const std::string& user, const std::string& password) {
    if (instance == nullptr) {
        instance = new DBConnection(host, port, user, password);
    }
    return instance;
}

mysqlx::Session& DBConnection::getSession() {
    return session;
}

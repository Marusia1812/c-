#pragma once

#include <mysqlx/xdevapi.h>
#include <string>

class DBConnection {
private:
    static DBConnection* instance;
    mysqlx::Session session;
    DBConnection(const std::string& host, unsigned int port, const std::string& user, const std::string& password);

public:
    static const std::string defaultSchemaName;
    static std::hash<std::string> hasher;

    // Delete copy constructor and assignment operation
    DBConnection(DBConnection const&) = delete;
    DBConnection& operator=(DBConnection const&) = delete;

    static DBConnection* getInstance(const std::string& host = "localhost", unsigned int port = 33060, const std::string& user = "root", const std::string& password = "toor");
    mysqlx::Session& getSession();
};
    
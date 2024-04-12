#pragma once

#include "DBConnection.h"

class DBStartup {
private:
    bool createSchema(const std::string& schemaName);
    void executeCommand(const std::string& sqlCommand);
    void insertDummyData();

public:
    DBStartup();
    void initializeDatabase();  
};

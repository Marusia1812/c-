#include <iostream>
#include <mysqlx/xdevapi.h>
#include "DBStartup.h"
#include "UI.h"
#include <stdlib.h>

int main()
{
    DBConnection* dbConnection = DBConnection::getInstance();
    mysqlx::Session& session = dbConnection->getSession();
    
    DBStartup dbStartup;
    dbStartup.initializeDatabase();
    
    UI ui;
    ui.clearConsole();
    ui.run();

    return 0;
}
#include "DBStartup.h"
#include <iostream>
#include <mysqlx/xdevapi.h> 
#include <functional>

DBStartup::DBStartup() {}

bool DBStartup::createSchema(const std::string& schemaName) {
    try {
        mysqlx::Session& session = DBConnection::getInstance()->getSession();
        bool schemaExists = session.getSchema(schemaName).existsInDatabase();
        if (!schemaExists) {
            session.createSchema(schemaName);
            std::cout << "Schema created: " << schemaName << std::endl;
            return true;
        }
    }
    catch (const mysqlx::Error& err) {
        std::cerr << "Error checking/creating schema: " << err.what() << std::endl;
    }

    return false;
}

void DBStartup::executeCommand(const std::string& sqlCommand) {
    try {
        mysqlx::Session& session = DBConnection::getInstance()->getSession();
        session.sql(sqlCommand).execute();
        std::cout << "Executed: " << sqlCommand << std::endl;
    }
    catch (const mysqlx::Error& err) {
        std::cerr << "Error executing command: " << err.what() << std::endl;
    }
}

void DBStartup::insertDummyData() {
    // Insert dummy data for Role table
    executeCommand("INSERT INTO Role (name) VALUES ('Administrator'), ('Librarian'), ('Customer');");

    // Insert dummy data for Library table
    executeCommand("INSERT INTO Library (name) VALUES ('Main Library'), ('West Branch'), ('East Branch');");

    // Insert dummy data for Genre table
    executeCommand("INSERT INTO Genre (name) VALUES ('Fiction'), ('Non-Fiction'), ('Science Fiction'), ('Fantasy'), ('Mystery');");

    // Insert dummy data for Author table
    executeCommand("INSERT INTO Author (fullname) VALUES "
        "('Agatha Christie'), ('J.K. Rowling'), ('Isaac Asimov'), "
        "('George Orwell'), ('J.R.R. Tolkien');");

    // Insert dummy data for Book table
    executeCommand("INSERT INTO Book (isbn, published_year, title, genre_id, library_id, person_id, status) VALUES "
        "('978-0439358071', 2003, 'Harry Potter and the Order of the Phoenix', 'Fiction', 1, NULL, 'Available'),"
        "('978-0451524935', 1949, '1984', 'Science Fiction', 1, NULL, 'Available'),"
        "('978-0261103573', 1954, 'The Fellowship of the Ring', 'Fantasy', 2, NULL, 'Available'),"
        "('978-0451526342', 1950, 'The Lion, the Witch and the Wardrobe', 'Fantasy', 3, NULL, 'Available'),"
        "('978-0006546061', 1945, 'Animal Farm', 'Science Fiction', 2, NULL, 'Available');");

    // Insert dummy data for Person table
    executeCommand("INSERT INTO Person (fullname, role, username, password) VALUES "
        "('John Doe', 'Customer', 'john.doe', '" + std::to_string(std::hash<std::string>{}("password1")) + "'), "
        "('Jane Smith', 'Librarian', 'jane.smith', '" + std::to_string(std::hash<std::string>{}("password2")) + "'),"
        "('Emily Johnson', 'Administrator', 'emily.johnson', '" + std::to_string(std::hash<std::string>{}("password3")) + "'),"
        "('Michael Brown', 'Customer', 'michael.brown', '" + std::to_string(std::hash<std::string>{}("password4")) + "'),"
        "('Linda Davis', 'Customer', 'linda.davis', '" + std::to_string(std::hash<std::string>{}("password5")) + "');");

    // Insert dummy data for BookAuthor table
    executeCommand("INSERT INTO BookAuthor (isbn, author_id) VALUES "
        "('978-0439358071', 2),"
        "('978-0451524935', 4),"
        "('978-0261103573', 5),"
        "('978-0451526342', 5),"
        "('978-0006546061', 4);");
}


void DBStartup::initializeDatabase() {
    bool newSchema = createSchema(DBConnection::defaultSchemaName);

    if (!newSchema) return;

    // Ensure session uses the created or existing schema
    mysqlx::Session& session = DBConnection::getInstance()->getSession();
    session.sql("USE " + DBConnection::defaultSchemaName).execute();

    // Role table
    executeCommand("CREATE TABLE IF NOT EXISTS Role ("
        "name ENUM('Administrator', 'Librarian', 'Customer') PRIMARY KEY"
        ");");

    // Person table
    executeCommand("CREATE TABLE IF NOT EXISTS Person ("
        "id INT AUTO_INCREMENT PRIMARY KEY, "
        "fullname VARCHAR(255) NOT NULL, "
        "username VARCHAR(255) NOT NULL UNIQUE, "
        "password VARCHAR(255) NOT NULL, "
        "role ENUM('Administrator', 'Librarian', 'Customer'), "
        "FOREIGN KEY (role) REFERENCES Role(name)"
        ");");

    // Library table
    executeCommand("CREATE TABLE IF NOT EXISTS Library ("
        "id INT AUTO_INCREMENT PRIMARY KEY, "
        "name VARCHAR(255) NOT NULL UNIQUE"
        ");");

    // Genre table
    executeCommand("CREATE TABLE IF NOT EXISTS Genre ("
        "name VARCHAR(255) PRIMARY KEY"
        ");");

    // Author table
    executeCommand("CREATE TABLE IF NOT EXISTS Author ("
        "id INT AUTO_INCREMENT PRIMARY KEY, "
        "fullname VARCHAR(255) NOT NULL, "
        "UNIQUE (fullname)"
        ");");

    // Book table
    executeCommand("CREATE TABLE IF NOT EXISTS Book ("
        "isbn VARCHAR(20) PRIMARY KEY, "
        "published_year INT NOT NULL, "
        "title VARCHAR(255) NOT NULL, "
        "genre_id VARCHAR(255), "
        "library_id INT, "
        "person_id INT, "
        "status ENUM('Available', 'Unavailable') NOT NULL, "
        "FOREIGN KEY (genre_id) REFERENCES Genre(name), "
        "FOREIGN KEY (library_id) REFERENCES Library(id), "
        "FOREIGN KEY (person_id) REFERENCES Person(id) ON DELETE SET NULL"
        ");");

    // BookAuthor table
    executeCommand("CREATE TABLE IF NOT EXISTS BookAuthor ("
        "isbn VARCHAR(20), "
        "author_id INT, "
        "PRIMARY KEY (isbn, author_id), "
        "FOREIGN KEY (isbn) REFERENCES Book(isbn) ON DELETE CASCADE, "
        "FOREIGN KEY (author_id) REFERENCES Author(id) ON DELETE CASCADE"
        ");");

    // Loan table
    executeCommand("CREATE TABLE IF NOT EXISTS Loan ("
        "book_id VARCHAR(20), "
        "person_id INT, "
        "loan_date DATE NOT NULL, "
        "expiration_date DATE NOT NULL, "
        "return_date DATE NULL DEFAULT NULL, "
        "PRIMARY KEY (book_id, person_id), "
        "FOREIGN KEY (book_id) REFERENCES Book(isbn) ON DELETE CASCADE, "
        "FOREIGN KEY (person_id) REFERENCES Person(id) ON DELETE CASCADE"
        ");");

    std::cout << "Database and tables initialized successfully." << std::endl;

    insertDummyData();

    std::cout << "Dummy data inserted into the database." << std::endl;
}

# Code Documentation

## Entity Classes (e.g., `Book.h/cpp`)

Entity classes represent database tables in code. Each instance of an entity corresponds to a row in its respective table.

### Key Components:
- **Attributes**: Represent the table columns. For a `Book` entity, these might include `isbn`, `title`, `publishedYear`, etc.
- **Constructors**: Used to instantiate objects with specific values.

### Data Access Objects (DAOs) (e.g., `BookDAO.h/cpp`)

DAOs abstract the underlying database operations for their associated entities.

### Key Methods:
- **Add/Create**: Inserts a new row into the table corresponding to the entity. For example, `addBook` adds a new book record to the `Book` table.
- **Get/Read**: Retrieves records from the database. Methods like `getBookByISBN` fetch a specific book, while others might retrieve a list of books meeting certain criteria.
- **Update**: Modifies an existing record in the database. The `updateBook` method updates the details of an existing book.
- **Delete**: Removes a record from the database. For instance, `deleteBook` would remove a book entry based on its ISBN.

## Database Connection Management (e.g., `DBConnection.h/cpp`)

Handles establishing and maintaining connections with the database.

### Key Features:
- **Singleton Pattern**: Ensures that only one instance of the connection object is created.
- **Connection Initialization**: Establishes a connection to the database using credentials and connection strings.
- **Session Management**: Provides methods to retrieve the current database session for executing queries.

### Database Initialization (e.g., `DBStartup.h/cpp`)

Responsible for setting up the database schema and populating tables with initial data.

### Key Actions:
- **Schema Creation**: Checks for the existence of the database schema and creates it if not present.
- **Table Creation**: Executes SQL commands to create necessary tables according to the application's data model.
- **Data Seeding**: Inserts initial data into tables.

## UI

The `UI` class is responsible for managing the user interface of the library management system. It handles user interactions, displays menus based on user roles (Administrator, Librarian, Customer), and facilitates operations like book management, user management, and loan transactions.

### Public Methods

- **UI()**: Constructor that initializes the UI system, setting the initial page to the welcome view and the user role to unauthorized.

- **void run()**: Main loop that drives the user interface, handling page navigation and executing actions based on user input.

- **void clearConsole()**: Clears the console window to keep the UI clean and readable.

- **void setNotification(const std::string& message)**: Sets a notification message to be displayed on the next UI screen, providing feedback or information to the user.

### Private Methods

- **int getInputRange(unsigned int start, unsigned int end)**: Prompts the user to enter a number within a specified range and validates the input.

- **UIPage getInput(const std::unordered_map<std::string, UIPage>& options)**: Displays a menu of options to the user and returns the selected UIPage based on user input.

- **bool verifyPassword(const std::string& password, const std::string& hashedPassword)**: Compares a user-entered password against a hashed password to verify access.

- **UserRole stringToUserRole(const std::string& role)**: Converts a string representation of a user role to the corresponding UserRole enum value.

- **std::string userRoleToString(const UserRole& role)**: Converts a UserRole enum value to its string representation.

- **BookStatus stringToBookStatus(const std::string& status)**: Converts a string representation of a book status to the corresponding BookStatus enum value.

- **std::string bookStatusToString(const BookStatus& status)**: Converts a BookStatus enum value to its string representation.

- **void showNotification()**: Displays any pending notification messages to the user.

- **bool createNewUser(const std::string& role)**: Guides the user through the process of creating a new user account with a specified role.

- **void printBooks(std::vector<Book>& books)**: Prints a list of books, showing their details.

- Various `showXXX` methods (**showWelcome**, **showAdminMenu**, etc.): Methods for displaying different menus and screens based on the current user's role and navigation choices.

### Enumerations

- **UIPage**: Defines possible UI pages/screens the user can navigate to, such as Welcome, AdminMenu, LibrarianMenu, etc.

- **UserRole**: Represents the roles a user can have in the system (Administrator, Librarian, Customer, Unauthorized).

- **BookStatus**: Indicates the availability status of a book (Available, Unavailable).

# Usage Instructions

### Configuring the Database Connection
- The file `DBConnection.cpp` specifies the correct access parameters to the MySQL database (host = "localhost", port = 33060, user = "root", password = "toor").

### Building and Launching the Application
- Build the project and launch it. Check if the application connects to the database successfully and works correctly.

## Steps for Preparing and Launching the Project

### Windows

#### Setting up the Development Environment
- Install Visual Studio.
- Install MySQL Server and MySQL Connector/C++ 8.0 on your operating system, following the official MySQL instructions:
  - [MySQL Server Download](https://dev.mysql.com/downloads/mysql/)
  - [MySQL Installation Guide](https://dev.mysql.com/doc/mysql-installation-excerpt/5.7/en/)

#### Configuring the Project in Visual Studio
- Open the solution file `project.sln` in Visual Studio.
- In the project properties, add the paths to the header files (include directories) and libraries (lib directories) of MySQL Connector/C++ in the appropriate sections of the C/C++ and Linker settings.
- Ensure that the project settings specify building with the same architecture (x86 or x64) as the installed MySQL Connector/C++.

### Ubuntu/Debian

#### Required packages

```bash
sudo apt-get install mysql-server
sudo apt-get install libmysqlcppconn-dev libmysqlcppconn8-2
sudo apt-get install libssl-dev
sudo apt-get install g++
```
#### build and launch
```bash
make
./program_debug
```
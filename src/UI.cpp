#include "UI.h"
#include <functional>
#include <signal.h>
#include <regex>
#include <sstream>
#include <ctime>
#include <iomanip>

#ifdef _WIN32
#define localtime_x(tm, time) localtime_s(tm, time)
#else
#define localtime_x(tm, time) localtime_r(time, tm)
#endif

constexpr time_t SECONDS_IN_WEEK = 7 * 24 * 60 * 60;

UI::UI() : currentPage(UIPage::ViewWelcome), currentRole(UserRole::Unauthorized) {}

void UI::clearConsole() {
    std::system("cls||clear");
}

bool UI::verifyPassword(const std::string& password, const std::string& hashedPassword) {
    return std::to_string(std::hash<std::string>{}(password)).compare(hashedPassword) == 0;
}

UserRole UI::stringToUserRole(const std::string& role) {
    if (role == "Administrator") {
        return UserRole::Administrator;
    } else if (role == "Librarian") {
        return UserRole::Librarian;
    } else if (role == "Customer") {
        return UserRole::Customer;
    }

    return UserRole::Unauthorized;
}

std::string UI::userRoleToString(const UserRole& role) {
    if (role == UserRole::Administrator) {
        return "Administrator";
    } else if (role == UserRole::Librarian) {
        return "Librarian";
    } else if (role == UserRole::Customer) {
        return "Customer";
    }

    return "Unauthorized";
}

BookStatus UI::stringToBookStatus(const std::string& status) {
    if (status == "Available") {
        return BookStatus::Available;
    }
    
    return BookStatus::Unavailable;
}

std::string UI::bookStatusToString(const BookStatus& status) {
    if (status == BookStatus::Available) {
        return "Available";
    }

    return "Unavailable";
}

void UI::showNotification() {
    if (!notification.empty()) {
        std::cout << "Notification: " << notification << "\n\n";
        notification.clear();
    }
}

void UI::setNotification(const std::string& message) {
    notification = message;
}

int UI::getInputRange(unsigned int start, unsigned int end) {
    int intInput;
    std::string input;
    std::cin >> input;

    try {
        intInput = std::stoi(input);
    }
    catch (...) {
        std::cout << "Invalid input format." << std::endl;
        return -1;
    }

    if (intInput < start || intInput > end) {
        std::cout << "Choice should be between the given range." << std::endl;
        return -1;
    }

    return intInput;
}

UIPage UI::getInput(const std::unordered_map<std::string, UIPage>& options) {
    std::vector<std::string> keys;
    for (auto it = options.begin(); it != options.end(); it++) {
        keys.push_back(it->first);
    }

    for (int i = 0; i < keys.size(); ++i) {
        std::cout << i + 1 << ". " << keys[i] << std::endl;
    }
    std::cout << "0. Exit" << std::endl;

    while (true) {
        std::cout << "Choice: ";
        int intInput = getInputRange(0, keys.size());

        if (intInput == -1) {
            continue;
        }

        if (intInput == 0) {
            return UIPage::Exit;
        }

        return options.find(keys[intInput - 1])->second;
    }
}

UIPage UI::showWelcome() {
    const std::unordered_map<std::string, UIPage> options = {
        {"Login", UIPage::Login},
        {"Register", UIPage::Registration}
    };
    return getInput(options);
}

UIPage UI::showLogin() {
    std::string username;
    std::cout << "Username: ";
    std::cin >> username;
    std::string password;
    std::cout << "Password: ";
    std::cin >> password;

    currentUser = PersonDAO::getPersonByUsername(username);
    if (currentUser == nullptr || !(verifyPassword(password, currentUser->password))) {
        setNotification("Wrong username or password");
        return UIPage::ViewWelcome;
    }

    currentRole = stringToUserRole(currentUser->role);

    if (currentRole == UserRole::Administrator) {
        return UIPage::ViewAdminMenu;
    }
    else if (currentRole == UserRole::Librarian) {
        return UIPage::ViewLibrarianMenu;
    }
    else if (currentRole == UserRole::Customer) {
        return UIPage::ViewCustomerMenu;
    }

    return UIPage::ViewWelcome;
}

UIPage UI::showLogout() {
    currentUser.reset();
    currentRole = UserRole::Unauthorized;

    return UIPage::ViewWelcome;
}

bool UI::createNewUser(const std::string& role) {
    std::string username;
    std::cout << "Username: ";
    std::cin >> username;

    auto existingPerson = PersonDAO::getPersonByUsername(username);
    if (existingPerson) {
        setNotification("Username taken");
        return false;
    }

    std::string password;
    std::cout << "Password: ";
    std::cin >> password;

    std::string fullname;
    std::cout << "Full Name: ";
    std::cin >> fullname;

    Person newPerson(fullname, role, username, password);
    PersonDAO::addPerson(newPerson);

    setNotification("New user created sucessfully!");

    return true;
}

UIPage UI::showRegistration(const std::string& role, const UIPage returnPage) {
    createNewUser(role);
    return returnPage;
}

UIPage UI::showAdminMenu() {
    const std::unordered_map<std::string, UIPage> options = {
        {"Show Users", UIPage::ViewShowUsers},
        {"New User", UIPage::ViewNewUser},
        {"Remove User", UIPage::ViewRemoveUser},
        {"Change User Role", UIPage::ViewUserRole},
        {"Logout", UIPage::Logout}
    };
    std::cout << "ADMIN MENU" << std::endl;
    return getInput(options);
}

UIPage UI::showUsers() {
    std::cout << "Showing Users" << std::endl;
    auto users = PersonDAO::getAll();
    for (int i = 0; i < users.size(); ++i) {
        std::cout << users[i] << std::endl;
    }
    std::cout << std::endl;

    const std::unordered_map<std::string, UIPage> options = {
        {"Main Menu", UIPage::ViewAdminMenu},
        {"New User", UIPage::ViewNewUser},
        {"Remove User", UIPage::ViewRemoveUser},
        {"Change User Role", UIPage::ViewUserRole}
    };

    return getInput(options);
}

UIPage UI::showNewUser() {
    const std::vector<std::string> options = { "Cancel", "Administrator", "Librarian", "Customer"};
    std::cout << "Create New User" << std::endl;
    for (int i = 0; i < options.size(); ++i) {
        std::cout << i + 1 << ". " << options[i] << std::endl;
    }

    while (true) {
        int intInput;
        std::cout << "Role: ";
        intInput = getInputRange(1, options.size());

        if (intInput == -1) {
            continue;
        }

        if (intInput == 1) {
            return UIPage::ViewAdminMenu;
        }

        return showRegistration(options[intInput - 1], UIPage::ViewAdminMenu);
    }
}

UIPage UI::showRemoveUser() {
    std::cout << "Remove User" << std::endl;
    auto users = PersonDAO::getAll();
    for (int i = 0; i < users.size(); ++i) {
        std::cout << i + 1 << ". " << users[i] << std::endl;
    }
    std::cout << std::endl;

    int intInput;
    while (true) {
        std::cout << "Pick (0 to cancel): ";
        intInput = getInputRange(0, users.size());

        if (intInput == -1) {
            continue;
        }

        break;
    }

    if (intInput == 0) {
        return UIPage::ViewAdminMenu;
    }

    PersonDAO::deletePerson(users[intInput - 1].id);
    setNotification("User sucessfully removed!");

    return UIPage::ViewAdminMenu;
}

UIPage UI::showUserRole() {
    std::cout << "Changing User Role" << std::endl;
    auto users = PersonDAO::getAll();
    for (int i = 0; i < users.size(); ++i) {
        std::cout << i + 1 << ". " << users[i] << std::endl;
    }
    std::cout << std::endl;

    int intInput;
    while (true) {
        std::cout << "Pick (0 to cancel): ";
        intInput = getInputRange(0, users.size());

        if (intInput == -1) {
            continue;
        }

        break;
    }

    if (intInput == 0) {
        return UIPage::ViewAdminMenu;
    }

    const std::vector<std::string> roles = { "Administrator", "Librarian", "Customer" };
    for (int i = 0; i < roles.size(); ++i) {
        std::cout << i + 1 << ". " << roles[i] << std::endl;
    }

    int intRole;
    while (true) {
        std::cout << "Change to role: ";
        intRole = getInputRange(1, roles.size());

        if (intRole == -1) {
            continue;
        }

        break;
    }

    users[intInput - 1].role = roles[intRole - 1];
    PersonDAO::updatePerson(users[intInput - 1]);
    setNotification("User role sucessfully changed!");

    return UIPage::ViewAdminMenu;
}

UIPage UI::showLibrarianMenu() {
    const std::unordered_map<std::string, UIPage> options = {
        {"New Book", UIPage::ViewNewBook},
        {"Remove Book", UIPage::ViewRemoveBook},
        {"Move Book", UIPage::ViewMoveBook},
        {"Logout", UIPage::Logout}
    };
    std::cout << "LIBRARIAN MENU" << std::endl;
    return getInput(options);
}

UIPage UI::showNewBook() {
    std::cout << "Adding New Book:" << std::endl;
    std::string isbn, title, author;
    int publishedYear, genre, library, status;

    do {
        std::cout << "ISBN: ";
        std::cin >> isbn;
    } while (isbn.empty());
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Title: ";
    std::getline(std::cin, title);

    do {
        std::cout << "Author: ";
        std::getline(std::cin, author);
    } while (author.empty());

    time_t now = time(0);
    struct tm ltm;
    localtime_x(&ltm, &now);

    std::cout << "Published Year (0 - " << 1900 + ltm.tm_year << "): ";
    publishedYear = getInputRange(0, 1900 + ltm.tm_year);

    std::cout << "Genre" << std::endl;
    auto allGenres = GenreDAO::getAll();
    for (int i = 0; i < allGenres.size(); ++i) {
        std::cout << i << ". " << allGenres[i].name << std::endl;
    }
    std::cout << "Input (0 - " << allGenres.size() - 1 << "): ";
    genre = getInputRange(0, allGenres.size() - 1);

    std::cout << "Library" << std::endl;
    auto allLibraries = LibraryDAO::getAll();
    for (int i = 0; i < allLibraries.size(); ++i) {
        std::cout << i << ". " << allLibraries[i].name << std::endl;
    }
    std::cout << "Input (0 - " << allLibraries.size() - 1 << "): ";
    library = getInputRange(0, allLibraries.size() - 1);

    std::cout << "Status" << std::endl;
    const std::vector<std::string> statuses = { "Available", "Unavailable" };
    for (int i = 0; i < statuses.size(); ++i) {
        std::cout << i << ". " << statuses[i] << std::endl;
    }
    std::cout << "Input (0 - " << statuses.size() - 1 << "): ";
    status = getInputRange(0, 1);

    auto book = Book(isbn, publishedYear, title, allGenres[genre].name, allLibraries[library].id, statuses[status]);
    BookDAO::addBook(book);

    auto existingAuthor = AuthorDAO::getAuthorByName(author);
    int authorId;
    if (!existingAuthor) {
        auto newAuthor = Author(author);
        authorId = AuthorDAO::addAuthor(newAuthor);
    }
    else {
        authorId = existingAuthor->id;
    }

    BookAuthorDAO::addBookAuthor(isbn, authorId);

    setNotification("Book sucessfully created!");

    return UIPage::ViewLibrarianMenu;
}

UIPage UI::showRemoveBook() {
    auto books = BookDAO::getAll();
    if (books.empty()) {
        setNotification("There are no books to remove.");
        return UIPage::ViewLibrarianMenu;
    }

    std::cout << "Removing Book" << std::endl;
    printBooks(books);
    std::cout << "Input (0 - " << books.size() - 1 << "): ";
    int choice = getInputRange(0, books.size() - 1);

    if (choice > 0) {
        try {
            BookDAO::deleteBook(books[choice].isbn);
        }
        catch (std::exception& e) {
            setNotification(e.what());
        }

        setNotification("Book sucessfully removed!");
    }

    return UIPage::ViewLibrarianMenu;
}

void UI::printBooks(std::vector<Book>& books) {
    for (int i = 0; i < books.size(); ++i) {
        std::cout << i
            << ". " << books[i].title
            << " | " << books[i].isbn
            << " | " << books[i].publishedYear
            << " | " << books[i].status
            << std::endl;
    }
}

UIPage UI::showMoveBook() {
    std::cout << "Moving Book" << std::endl;
    auto books = BookDAO::getAllAvailable();
    if (books.empty()) {
        setNotification("There are no books to move.");
        return UIPage::ViewLibrarianMenu;
    }

    printBooks(books);
    std::cout << "Input (0 - " << books.size() - 1 << "): ";
    int bookChoice = getInputRange(0, books.size() - 1);

    if (bookChoice == -1) {
        return UIPage::ViewLibrarianMenu;
    }

    auto libraries = LibraryDAO::getAll();
    for (int i = 0; i < libraries.size(); ++i) {
        std::cout << i << ". " << libraries[i].name;
        if (books[bookChoice].libraryId == libraries[i].id) {
            std::cout << " (current)";
        }
        std::cout << std::endl;
    }
    std::cout << "Input (0 - " << libraries.size() - 1 << "): ";
    int libraryChoice = getInputRange(0, libraries.size() - 1);

    if (libraryChoice == -1) {
        return UIPage::ViewLibrarianMenu;
    }

    books[bookChoice].libraryId = libraries[libraryChoice].id;

    try {
        BookDAO::updateBook(books[bookChoice]);
    }
    catch (std::exception& e) {
        setNotification(e.what());
    }

    setNotification("Book sucessfully moved!");

    return UIPage::ViewLibrarianMenu;
}

UIPage UI::showCustomerMenu() {
    const std::unordered_map<std::string, UIPage> options = {
        {"View Books", UIPage::ViewShowBooks},
        {"Loan Book", UIPage::ViewLoanBook},
        {"Return Book", UIPage::ViewReturnBook},
        {"Logout", UIPage::Logout}
    };
    std::cout << "CUSTOMER MENU" << std::endl;
    return getInput(options);
}

UIPage UI::showBooks() {
    std::cout << "Viewing Available Books" << std::endl;
    auto books = BookDAO::getAllAvailable();
    printBooks(books);

    std::cout << "Press Enter to continue . . ." << std::flush;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    return UIPage::ViewCustomerMenu;
}

UIPage UI::showLoanBook() {
    auto books = BookDAO::getAllAvailable();
    if (books.empty()) {
        setNotification("There are no available books to loan.");
        return UIPage::ViewCustomerMenu;
    }

    std::cout << "Loan Book" << std::endl;
    printBooks(books);
    std::cout << "Input (0 - " << books.size() - 1 << "): ";
    int bookChoice;
    do {
        bookChoice = getInputRange(0, books.size() - 1);
    } while (bookChoice < 0);

    std::string startDateString;
    std::regex dateFormat(R"(^\d{4}\-\d{2}\-\d{2}$)");
    tm tm = {};
    time_t startDate;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    do {
        std::cout << "Starting date (yyyy-mm-dd): ";
        std::getline(std::cin, startDateString);

        if (std::regex_match(startDateString, dateFormat)) {
            std::istringstream ss(startDateString);
            ss >> std::get_time(&tm, "%Y-%m-%d");

            if (!ss.fail()) {
                startDate = mktime(&tm);
                break;
            }
        }
        else {
            std::cout << "Invalid format. Please try again." << std::endl;
        }
    } while (true);

    std::cout << "For how many weeks (1 - 8): ";
    int duration = getInputRange(1, 8);

    time_t expirationDate = startDate + duration * SECONDS_IN_WEEK;
    struct tm ptm;
    localtime_x(&ptm, &expirationDate);
    char buffer[16];
    std::strftime(buffer, 16, "%Y-%m-%d", &ptm);
    std::string expirationDateString(buffer);
   
    auto loan = Loan(books[bookChoice].isbn, currentUser->id, startDateString, expirationDateString);
    LoanDAO::addLoan(loan);

    auto book = BookDAO::getBookByISBN(books[bookChoice].isbn);
    book->status = "Unavailable";
    BookDAO::updateBook(*book);

    setNotification("Book sucessfuly loaned!");

    return UIPage::ViewCustomerMenu;
}

UIPage UI::showReturnBook() {
	auto loans = LoanDAO::getLoansByPersonId(currentUser->id);
    if (loans.empty()) {
        setNotification("There are no loaned books to return.");
        return UIPage::ViewCustomerMenu;
    }

	std::cout << "Return Book" << std::endl;
    for (int i = 0; i < loans.size(); ++i) {
        auto book = BookDAO::getBookByISBN(loans[i].bookId);
        std::cout << i
            << ". " << book->isbn
            << " - " << book->title
            << " | " << loans[i].loanDate
            << " | " << loans[i].expirationDate
            << std::endl;
    }
	std::cout << "Input (-1 to return, 0 - " << loans.size() - 1 << "): ";
    const int loanChoice = getInputRange(0, loans.size() - 1);

	if (loanChoice == -1) {
		return UIPage::ViewCustomerMenu;
	}

    const auto book = BookDAO::getBookByISBN(loans[loanChoice].bookId);
    book->status = "Available";
    BookDAO::updateBook(*book);

    LoanDAO::deleteLoan(loans[loanChoice].bookId, currentUser->id);

    setNotification("Book sucessfully returned!");

    return UIPage::ViewCustomerMenu;
}

void UI::run() {
    while (currentPage != UIPage::Exit) {
        clearConsole();

        showNotification();
        switch (currentPage) {
        case UIPage::ViewWelcome: currentPage = showWelcome(); break;
        case UIPage::ViewAdminMenu: currentPage = showAdminMenu(); break;
        case UIPage::ViewShowUsers: currentPage = showUsers(); break;
        case UIPage::ViewNewUser: currentPage = showNewUser(); break;
        case UIPage::ViewRemoveUser: currentPage = showRemoveUser(); break;
        case UIPage::ViewUserRole: currentPage = showUserRole(); break;
        case UIPage::ViewLibrarianMenu: currentPage = showLibrarianMenu(); break;
        case UIPage::ViewNewBook: currentPage = showNewBook(); break;
        case UIPage::ViewRemoveBook: currentPage = showRemoveBook(); break;
        case UIPage::ViewMoveBook: currentPage = showMoveBook(); break;
        case UIPage::ViewCustomerMenu: currentPage = showCustomerMenu(); break;
        case UIPage::ViewShowBooks: currentPage = showBooks(); break;
        case UIPage::ViewLoanBook: currentPage = showLoanBook(); break;
        case UIPage::ViewReturnBook: currentPage = showReturnBook(); break;
        case UIPage::Login: currentPage = showLogin(); break;
        case UIPage::Logout: currentPage = showLogout(); break;
        case UIPage::Registration: currentPage = showRegistration("Customer", UIPage::ViewWelcome); break;
        default: currentPage = UIPage::Exit;
        }
    }
}

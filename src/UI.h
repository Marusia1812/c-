#ifndef UI_H
#define UI_H

#include "UIPage.h"
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "UserRole.h"
#include "BookStatus.h"

#include "Person.h"
#include "PersonDAO.h"
#include "Book.h"
#include "BookDAO.h"
#include "Author.h"
#include "AuthorDAO.h"
#include "Genre.h"
#include "GenreDAO.h"
#include "Loan.h"
#include "LoanDAO.h"
#include "BookAuthor.h"
#include "BookAuthorDAO.h"
#include "Library.h"
#include "LibraryDAO.h"

class UI {
private:
    static int getInputRange(unsigned int start, unsigned int end);
    static UIPage getInput(const std::unordered_map<std::string, UIPage>& options);
    std::string notification;
    std::unique_ptr<Person> currentUser;
    UIPage currentPage;
    UserRole currentRole;

    bool verifyPassword(const std::string& password, const std::string& hashedPassword);
    UserRole stringToUserRole(const std::string& role);
    std::string userRoleToString(const UserRole& role);
    BookStatus stringToBookStatus(const std::string& status);
    std::string bookStatusToString(const BookStatus& status);

    void showNotification();
    bool createNewUser(const std::string& role);

    void printBooks(std::vector<Book>& books);

    UIPage showWelcome();
    UIPage showAdminMenu();
    UIPage showUsers();
    UIPage showNewUser();
    UIPage showRemoveUser();
    UIPage showUserRole();
    UIPage showLibrarianMenu();
    UIPage showNewBook();
    UIPage showRemoveBook();
    UIPage showMoveBook();
    UIPage showCustomerMenu();
    UIPage showBooks();
    UIPage showLoanBook();
    UIPage showLogin();
    UIPage showLogout();
    UIPage showReturnBook();
    UIPage showRegistration(const std::string& role, const UIPage returnPage);
public:
    UI();
    void run();
    void clearConsole();
    void setNotification(const std::string& message);
};

#endif

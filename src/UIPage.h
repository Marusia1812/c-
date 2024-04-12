#ifndef UIPAGE_H
#define UIPAGE_H

enum class UIPage {
    // All Users
    Exit,
    Login,
    Registration,
    Logout,
    ViewWelcome,

    // Customer
    ViewCustomerMenu,
    ViewShowBooks,
    ViewLoanBook,
    ViewReturnBook,

    // Librarian
    ViewLibrarianMenu,
    ViewNewBook,
    ViewRemoveBook,
    ViewMoveBook,

    // Administrator
    ViewAdminMenu,
    ViewShowUsers,
    ViewNewUser,
    ViewRemoveUser,
    ViewUserRole
};

#endif
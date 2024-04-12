#include "Loan.h"

Loan::Loan(const std::string& bookId, uint64_t personId, const std::string& loanDate,
    const std::string& expirationDate, const std::string& returnDate)
    : bookId(bookId), personId(personId), loanDate(loanDate),
    expirationDate(expirationDate), returnDate(returnDate) {}

Loan::Loan(const std::string& bookId, uint64_t personId, const std::string& loanDate,
    const std::string& expirationDate)
    : bookId(bookId), personId(personId), loanDate(loanDate),
    expirationDate(expirationDate) {}

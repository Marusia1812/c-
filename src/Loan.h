#ifndef LOAN_H
#define LOAN_H

#include <string>

class Loan {
public:
    std::string bookId;
    uint64_t personId;
    std::string loanDate;
    std::string expirationDate;
    std::string returnDate;

    Loan(const std::string& bookId, uint64_t personId, const std::string& loanDate,
        const std::string& expirationDate, const std::string& returnDate);

    Loan(const std::string& bookId, uint64_t personId, const std::string& loanDate,
        const std::string& expirationDate);
};

#endif

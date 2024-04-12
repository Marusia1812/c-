#ifndef LOANDAO_H
#define LOANDAO_H

#include "Loan.h"
#include "DBConnection.h"
#include <memory>
#include <iostream>

class LoanDAO {
public:
    static std::string tmToString(const std::tm& tm);
    static void addLoan(const Loan& loan);
    static std::unique_ptr<Loan> getLoan(const std::string& bookId, uint32_t personId);
    static void updateLoan(const Loan& loan);
    static void deleteLoan(const std::string& bookId, uint64_t personId);
    static std::vector<Loan> getLoansByPersonId(uint64_t personId);
};

#endif
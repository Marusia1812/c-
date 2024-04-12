#include "LoanDAO.h"
#include <iomanip>
#include <sstream>

void LoanDAO::addLoan(const Loan& loan) {
    try {
        auto& session = DBConnection::getInstance()->getSession();
        auto table = session.getSchema(DBConnection::defaultSchemaName).getTable("Loan");
        table.insert("book_id", "person_id", "loan_date", "expiration_date")
            .values(loan.bookId, loan.personId, loan.loanDate, loan.expirationDate)
            .execute();
    }
    catch (std::exception& e) {
        std::cout << e.what();
    }
}

std::unique_ptr<Loan> LoanDAO::getLoan(const std::string& bookId, uint32_t personId) {
    auto& session = DBConnection::getInstance()->getSession();
    auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("Loan")
        .select("book_id", "person_id", "loan_date", "expiration_date", "return_date")
        .where("book_id = :book_id AND person_id = :person_id")
        .bind("book_id", bookId)
        .bind("person_id", personId)
        .execute();
    auto row = result.fetchOne();
    if (!row.isNull()) {
        return std::make_unique<Loan>(row[0].get<std::string>(), row[1].get<uint32_t>(),
            row[2].get<std::string>(), row[3].get<std::string>(), row[4].get<std::string>());
    }
    return nullptr;
}

void LoanDAO::updateLoan(const Loan& loan) {
    auto& session = DBConnection::getInstance()->getSession();
    session.getSchema(DBConnection::defaultSchemaName).getTable("Loan")
        .update()
        .set("loan_date", loan.loanDate)
        .set("expiration_date", loan.expirationDate)
        .set("return_date", loan.returnDate)
        .where("book_id = :book_id AND person_id = :person_id")
        .bind("book_id", loan.bookId)
        .bind("person_id", loan.personId)
        .execute();
}

void LoanDAO::deleteLoan(const std::string& bookId, uint64_t personId) {
    auto& session = DBConnection::getInstance()->getSession();
    session.getSchema(DBConnection::defaultSchemaName).getTable("Loan")
        .remove()
        .where("book_id = :book_id AND person_id = :person_id")
        .bind("book_id", bookId)
        .bind("person_id", personId)
        .execute();
}

std::vector<Loan> LoanDAO::getLoansByPersonId(uint64_t personId) {
    try {
	    std::vector<Loan> loans;
	    auto& session = DBConnection::getInstance()->getSession();
	    auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("Loan")
		    .select("book_id", "person_id", "CAST(loan_date AS char) AS loan_date", "CAST(expiration_date AS char) AS expiration_date", "CAST(return_date AS char) AS return_date")
		    .where("person_id = :person_id")
		    .bind("person_id", personId)
		    .execute();
        for (auto row : result) {
            std::string bookId = row[0].get<std::string>();
            uint64_t personId = row[1].get<uint64_t>();
            std::string loanDate = row[2].get<std::string>();
            std::string expirationDate = row[3].get<std::string>();
            std::string returnDate = row[4].isNull() ? "" : row[4].get<std::string>();

            loans.emplace_back(bookId, personId, loanDate, expirationDate, returnDate);
        }
	    return loans;
    } catch (std::exception& e) {
        std::cout << e.what();
    }
}
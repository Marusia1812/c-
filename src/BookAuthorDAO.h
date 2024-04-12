#ifndef BOOKAUTHORDAO_H
#define BOOKAUTHORDAO_H

#include "DBConnection.h"
#include <string>
#include <vector>

class BookAuthorDAO {
public:
    static void addBookAuthor(const std::string& isbn, uint32_t authorId);
    static void deleteBookAuthor(const std::string& isbn, uint32_t authorId);
    static std::vector<uint32_t> getAuthorsForBook(const std::string& isbn);
    static std::vector<std::string> getBooksForAuthor(uint32_t authorId);
};

#endif

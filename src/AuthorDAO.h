#ifndef AUTHORDAO_H
#define AUTHORDAO_H

#include "Author.h"
#include <memory>
#include <vector>
#include "DBConnection.h"

class AuthorDAO {
public:
    static uint32_t addAuthor(const Author& author);
    static std::unique_ptr<Author> getAuthorById(uint32_t id);
    static std::unique_ptr<Author> getAuthorByName(std::string& name);
    static void updateAuthor(const Author& author);
    static void deleteAuthor(uint32_t id);
};

#endif
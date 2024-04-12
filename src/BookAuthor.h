#ifndef BOOKAUTHOR_H
#define BOOKAUTHOR_H

#include <string>

class BookAuthor {
public:
    std::string isbn;
    int authorId;

    BookAuthor(const std::string& isbn, int authorId);
};

#endif

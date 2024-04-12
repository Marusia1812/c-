#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book {
public:
    std::string isbn;
    int publishedYear;
    std::string title;
    std::string genreId;
    int libraryId;
    std::string status;
    uint32_t personId;

    Book(const std::string& isbn, int publishedYear, const std::string& title,
        const std::string& genreId, int libraryId, const std::string& status);


    Book(const std::string& isbn, int publishedYear, const std::string& title,
        const std::string& genreId, int libraryId, const std::string& status, uint32_t personId);
};

#endif

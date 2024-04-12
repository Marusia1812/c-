#include "Book.h"

Book::Book(const std::string& isbn, int publishedYear, const std::string& title,
    const std::string& genreId, int libraryId, const std::string& status)
    : isbn(isbn), publishedYear(publishedYear), title(title),
    genreId(genreId), libraryId(libraryId), status(status), personId(0) {}

Book::Book(const std::string& isbn, int publishedYear, const std::string& title,
    const std::string& genreId, int libraryId, const std::string& status, uint32_t personId)
    : isbn(isbn), publishedYear(publishedYear), title(title),
    genreId(genreId), libraryId(libraryId), status(status), personId(personId) {}

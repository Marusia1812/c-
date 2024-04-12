#ifndef LIBRARYDAO_H
#define LIBRARYDAO_H

#include "Library.h"
#include "DBConnection.h"
#include <memory>
#include <vector>

class LibraryDAO {
public:
    static uint32_t addLibrary(const Library& library);
    static std::unique_ptr<Library> getLibraryById(uint32_t id);
    static void updateLibrary(const Library& library);
    static void deleteLibrary(uint32_t id);
    static std::vector<Library> getAll();
};

#endif
#ifndef GENREDAO_H
#define GENREDAO_H

#include "Genre.h"
#include <memory>
#include "DBConnection.h"

class GenreDAO {
public:
    static std::string addGenre(const Genre& genre);
    static std::unique_ptr<Genre> getGenreByName(const std::string& name);
    static void updateGenre(const std::string& oldName, const std::string& newName);
    static void deleteGenre(const std::string& name);
    static std::vector<Genre> getAll();
};

#endif

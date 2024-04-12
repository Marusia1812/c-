#include "Author.h"

Author::Author(const std::string& fullname) : fullname(fullname) {}
Author::Author(uint32_t id, const std::string& fullname) : id(id), fullname(fullname) {}
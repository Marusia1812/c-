#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>

class Author {
public:
    uint32_t id;
    std::string fullname;

    Author(const std::string& fullname);
    Author(uint32_t id, const std::string& fullname);
};

#endif

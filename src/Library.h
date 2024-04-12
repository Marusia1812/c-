#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>

class Library {
public:
    uint32_t id;
    std::string name;

    Library(const std::string& name);
    Library(uint32_t id, const std::string& name);
};

#endif

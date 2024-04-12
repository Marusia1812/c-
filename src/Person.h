#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <functional>

class Person {
public:
    uint64_t id;
    std::string fullname;
    std::string role;
    std::string username;
    std::string password;

    Person(const std::string& fullname, const std::string& role,
        const std::string& username, const std::string& password);
    Person(uint64_t id, const std::string& fullname, const std::string& role,
        const std::string& username, const std::string& password);

    friend std::ostream& operator<<(std::ostream& os, const Person& person);
};

#endif

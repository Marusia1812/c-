#include "Person.h"
#include <iostream>

Person::Person(const std::string& fullname, const std::string& role,
    const std::string& username, const std::string& password)
    : fullname(fullname), role(role), username(username), password(std::to_string(std::hash<std::string>{}(password))) {}

Person::Person(uint64_t id, const std::string& fullname, const std::string& role,
    const std::string& username, const std::string& password)
    : id(id), fullname(fullname), role(role), username(username), password(password) {}

std::ostream& operator<<(std::ostream& os, const Person& person) {
    os << "ID: " << person.id << ", Full Name: " << person.fullname
        << ", Role: " << person.role << ", Username: " << person.username;
    return os;
}

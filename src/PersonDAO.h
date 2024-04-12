#ifndef PERSONDAO_H
#define PERSONDAO_H

#include "Person.h"
#include <memory>
#include <vector>

class PersonDAO {
public:
    static uint64_t addPerson(const Person& person);
    static std::unique_ptr<Person> getPersonById(uint64_t id);
    static std::vector<Person> getAll();
    static std::unique_ptr<Person> getPersonByUsername(const std::string& username);
    static void updatePerson(const Person& person);
    static bool deletePerson(uint64_t id);
};

#endif

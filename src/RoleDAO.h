#ifndef ROLEDAO_H
#define ROLEDAO_H

#include "Role.h"
#include <memory>
#include <vector>

class RoleDAO {
public:
    static std::unique_ptr<Role> getRoleByName(const std::string& name);
};

#endif

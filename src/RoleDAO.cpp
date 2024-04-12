#include "RoleDAO.h"
#include "DBConnection.h"

std::unique_ptr<Role> RoleDAO::getRoleByName(const std::string& name) {
    auto& session = DBConnection::getInstance()->getSession();
    auto result = session.getSchema(DBConnection::defaultSchemaName).getTable("Role")
        .select("name")
        .where("name = :name")
        .bind("name", name)
        .execute();
    auto row = result.fetchOne();
    if (!row.isNull()) {
        return std::make_unique<Role>(row[0].get<std::string>());
    }
    return nullptr;
}
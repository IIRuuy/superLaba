#pragma once

#include <string>
#include <set>
#include <stdexcept>
#include "RoleEntity.hpp"
#include "json.hpp"

class UserEntity {
public:
    UserEntity(unsigned int id, std::string name, unsigned int age);
    UserEntity(unsigned int id, std::string username, std::string name, std::string password, int age, std::set<RoleEntity> roles);
    UserEntity(std::string username, std::string password, std::string name, unsigned int age);
    UserEntity(std::string name, unsigned int age);
    std::string toString();

    std::string getUsername();
    std::string getPassword();
    std::string getName();
    std::set<RoleEntity> getRoles();
    unsigned int getId();

    void setId(unsigned int id);
    void setRoles(std::set<RoleEntity> roles);
    void setAge(unsigned int age);
    void setName(std::string name);

    nlohmann::json toJSON();
private:
    unsigned int _id;
    std::string _username;
    std::string _name;
    unsigned int _age;
    std::string _password;
    std::set <RoleEntity> _roles;
};
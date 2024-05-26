#include "UserEntity.hpp"

UserEntity::UserEntity(unsigned int id, std::string name, unsigned int age)
{
    _id = id;
    _name = name;
    _age = age;
}


UserEntity::UserEntity(unsigned int id, std::string username, std::string name, std::string password, int age, std::set<RoleEntity> roles)
{
    _id = id;
    _username = username;
    _password = password;
    _roles = roles;
    _name = name;
    _age = age;
}

UserEntity::UserEntity(std::string name, unsigned int age)
{
    _id = -1;
    _name = name;
    _age = age;
}

UserEntity::UserEntity(std::string username, std::string password, std::string name, unsigned int age) {
    _username = username;
    _password = password;
    _name = name;
    _age = age;
}

void UserEntity::setId(unsigned int id){
    _id = id;
}

std::string UserEntity::toString()
{
    return "Id: " + std::to_string(_id) + ", Username: " + _username + ", Name: " + _name + ", Age: " + std::to_string(_age);
}

std::string UserEntity::getUsername() {
    return _username;
}

std::string UserEntity::getPassword() {
    return _password;
}

std::set<RoleEntity> UserEntity::getRoles(){
    return _roles;
}

unsigned int UserEntity::getId() {
    return _id;
}

void UserEntity::setRoles(std::set<RoleEntity> roles) {
    _roles = roles;
}

void UserEntity::setAge(unsigned int age) {
    if(age > 150)
        throw std::invalid_argument("Uncorrect user age!"); 
    _age = age;
}

void UserEntity::setName(std::string name) {
    _name = name;
}

std::string UserEntity::getName() {
    return _name;
}

nlohmann::json UserEntity::toJSON() {
    nlohmann::json response;
    
    response["id"] = _id;
    response["username"] = _username;
    response["name"] = _name;
    response["age"] = _age;
    response["roles"] = _roles;
    
    return response;
}

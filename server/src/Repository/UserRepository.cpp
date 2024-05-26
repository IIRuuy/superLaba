#include "UserRepository.h"
#include <stdexcept>
#include <mutex>
#include <iostream>

std::mutex data_mutex;
std::map<unsigned int, UserEntity> users;

UserRepository::UserRepository() {
    std::set<RoleEntity> roles;
    roles.insert(RoleEntity::ADMIN);
    
    UserEntity user(0, "root", "root", "123", 0, roles);
    users.insert({0, user});
}

UserEntity UserRepository::addUser(UserEntity user) {
    std::unique_lock<std::mutex> lck (data_mutex);
    unsigned int id = !users.empty() ? users.rbegin()->first+1 : 0;
    user.setId(id);
    
    std::set<RoleEntity> roles;
    roles.insert(RoleEntity::USER);
    user.setRoles(roles);

    users.insert({id, user});

    return user;
}

bool UserRepository::deleteUser(unsigned int id) {
    std::unique_lock<std::mutex> lck (data_mutex);
    return users.erase(id);
}

std::map<unsigned int, UserEntity> UserRepository::getAllUsers() {
    std::unique_lock<std::mutex> lck (data_mutex);
    return users;
}

UserEntity UserRepository::getUser(unsigned int id) {
    std::unique_lock<std::mutex> lck (data_mutex);

    auto it = users.find(id);
    if(it == users.end())
        throw std::invalid_argument("Not found user"); 
    return it->second;
}

UserEntity UserRepository::getByUsername(std::string username) {
    std::unique_lock<std::mutex> lck (data_mutex);

    for (std::map<unsigned int, UserEntity>::iterator it = users.begin(); it != users.end(); ++it)
    {
        if(it->second.getUsername() == username)
            return it->second;
    }
    
    throw std::invalid_argument("Not found user"); 
}

bool UserRepository::existByUsername(std::string username) {
    std::unique_lock<std::mutex> lck (data_mutex);

    for (std::map<unsigned int, UserEntity>::iterator it = users.begin(); it != users.end(); ++it)
        if(it->second.getUsername() == username)
            return true;

    return false;
}

UserEntity UserRepository::changeNameAndAgeById(unsigned int id, std::string name, unsigned int age) {
    UserEntity user = this->getUser(id);

    user.setName(name);
    user.setAge(age);

    users.erase(id);
    users.insert({id, user});

    return user;
}

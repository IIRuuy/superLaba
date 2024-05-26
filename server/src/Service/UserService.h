#pragma once

#include "../Repository/UserRepository.h"
class UserService {
public:
    UserEntity addUser(UserEntity user);
    UserEntity getUser(unsigned int id);
    UserEntity getUser(std::string username);
    std::map<unsigned int, UserEntity> getAllUsers();
    bool deleteUser(unsigned int id);
    UserEntity changeNameAndAgeById(unsigned int id, std::string name, unsigned int age);

};
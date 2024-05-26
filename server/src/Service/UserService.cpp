#include "UserService.h"
#include <stdexcept>

UserRepository userRepository;

UserEntity UserService::addUser(UserEntity user) {
    if(!userRepository.existByUsername(user.getUsername()))
        return userRepository.addUser(user);
    throw std::invalid_argument("User with this username already exist!"); 
}

bool UserService::deleteUser(unsigned int id) {
    return userRepository.deleteUser(id);
}

std::map<unsigned int, UserEntity> UserService::getAllUsers() {
    return userRepository.getAllUsers();
}

UserEntity UserService::getUser(unsigned int id) {
    return userRepository.getUser(id);
}

UserEntity UserService::getUser(std::string username) {
    return userRepository.getByUsername(username);
}

UserEntity UserService::changeNameAndAgeById(unsigned int id, std::string name, unsigned int age) {
    return userRepository.changeNameAndAgeById(id, name, age);
}

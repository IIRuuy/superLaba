#include <map>
#include "../Entity/UserEntity.hpp"

class UserRepository {
public:
    UserRepository();
    UserEntity addUser(UserEntity user);
    UserEntity getUser(unsigned int id);
    std::map<unsigned int, UserEntity> getAllUsers();
    bool deleteUser(unsigned int id);
    UserEntity getByUsername(std::string username);
    bool existByUsername(std::string username);
    UserEntity changeNameAndAgeById(unsigned int id, std::string name, unsigned int age);
};
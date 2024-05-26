#include "../Entity/UserEntity.hpp"
#include "./UserService.h"

class AuthenticationService {
public:
    UserEntity signin(std::string basic_auth);
    AuthenticationService(UserService userService);
private:
    UserService _userService;
};
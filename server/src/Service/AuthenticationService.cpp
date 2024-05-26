#include "AuthenticationService.hpp"
#include "Utility.hpp"
#include <stdexcept>
#include <iostream>

AuthenticationService::AuthenticationService(UserService userService) {
    _userService = userService;
}

UserEntity AuthenticationService::signin(std::string basic_auth) {
    auto splited_header = utils::Split(basic_auth, " ");


    if (splited_header.size() == 2 && splited_header.front() == "Basic")
    {
        auto decode = utils::DecodeBase64(splited_header.back());
        auto splited_auth = utils::Split(decode, ":");

        if (splited_auth.size() == 2)
        {
            UserEntity userEntity = _userService.getUser(splited_auth.front());
            if(userEntity.getPassword() == splited_auth.back())
                return userEntity;
        }
    }
    throw std::invalid_argument("Bad request!"); 
}
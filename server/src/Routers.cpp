#include "Routers.hpp"
#include "Utility.hpp"
#include "Service/UserService.h"
#include "Service/AuthenticationService.hpp"
UserService userService;
AuthenticationService authenticationService(userService);

void route::RegisterResources(hv::HttpService &router)
{
router.POST("/signin", [](HttpRequest *req, HttpResponse *resp)
    {
        auto basic_auth = req->GetHeader("Authorization");

        if (!basic_auth.empty())
        {
            try {
                nlohmann::json response;
                
                UserEntity authUserEntity = authenticationService.signin(basic_auth);
                std::set<RoleEntity> rolesAuthUserEntity = authUserEntity.getRoles();
                resp->SetBody(authUserEntity.toJSON().dump());
                resp->content_type = APPLICATION_JSON;
                return 200;
            } 
            catch(const std::exception& e)
            {
            }
        }
        nlohmann::json response;
        response["error"] = "Bad credentials!";
        resp->SetBody(response.dump());
        resp->content_type = APPLICATION_JSON;
        return 400;
    });


    router.GET("/user", [](HttpRequest *req, HttpResponse *resp)    
    {
        auto basic_auth = req->GetHeader("Authorization");

        if (!basic_auth.empty())
        {
            try {
                UserEntity userEntity = authenticationService.signin(basic_auth);

                std::set<RoleEntity> roles = userEntity.getRoles();
        
                auto it = roles.find(RoleEntity::ADMIN);
                if(it == roles.end()) {
                    nlohmann::json response;
                    response["error"] = "Forbidden!";
                    resp->SetBody(response.dump());
                    resp->content_type = APPLICATION_JSON;
                    return 403;
                }
                std::map<unsigned int, UserEntity> users = userService.getAllUsers();
                nlohmann::json usersJson = nullptr;
                if(!users.empty()) {
                    for (const auto& pair : users) {
                        usersJson[std::to_string(pair.first)] = ((UserEntity)(pair.second)).toJSON();
                    }
                }

                resp->SetBody(usersJson.dump());
                resp->content_type = APPLICATION_JSON;
                return 200;
            } 
            catch(const std::exception& e)
            {
                nlohmann::json response;
                response["error"] = e.what();
                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
                return 401;
            }
        }

        nlohmann::json response;
        response["error"] = "Not authentication!";
        resp->SetBody(response.dump());
        resp->content_type = APPLICATION_JSON;
        return 401;
    });

    router.POST("/user", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json request;
        nlohmann::json response;
        
        try
        {
            request = nlohmann::json::parse(req->body);
            UserEntity user(request["username"], request["password"], request["name"], (unsigned int) request["age"]);
            try {

                resp->SetBody(userService.addUser(user).toJSON().dump());
                resp->content_type = APPLICATION_JSON;
                return 201;
            }
            catch (const std::exception& e)
            {
                response["error"] = e.what();
                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
                return 400;
            }
        }
        catch(const std::exception& e)
        {
            response["error"] = "Invalid JSON";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 400;
        }
    });

    router.GET("/user/{userId}", [](HttpRequest *req, HttpResponse *resp)
    {
        auto basic_auth = req->GetHeader("Authorization");

        if (!basic_auth.empty())
        {
            try {
                nlohmann::json response;
                int id;
                
                UserEntity authUserEntity = authenticationService.signin(basic_auth);
                std::set<RoleEntity> rolesAuthUserEntity = authUserEntity.getRoles();
                auto it = rolesAuthUserEntity.find(RoleEntity::ADMIN);
                try {
                    std::stringstream ss(req->GetParam("userId"));
                    if (ss >> id) {
                        if(id < 0) {
                            response["error"] = "Id must be unsigned number";
                            resp->SetBody(response.dump());
                            resp->content_type = APPLICATION_JSON;
                            return 400;
                        }

                        if((unsigned int)id == authUserEntity.getId() || it != rolesAuthUserEntity.end()) {
                            try {
                                UserEntity user = userService.getUser(id);
                                resp->SetBody(user.toJSON().dump());
                                resp->content_type = APPLICATION_JSON;
                                return 200;
                            }
                            catch(const std::exception& e)
                            {
                                nlohmann::json response;
                                response["error"] = e.what();
                                resp->SetBody(response.dump());
                                resp->content_type = APPLICATION_JSON;
                                return 404;
                            }
                            
                        } else {
                            nlohmann::json response;
                            response["error"] = "Forbidden!";
                            resp->SetBody(response.dump());
                            resp->content_type = APPLICATION_JSON;
                            return 403;
                        }

                    } 
                }
                catch(const std::exception& e)
                {
                    nlohmann::json response;
                    response["error"] = "Resourse not found!";
                    resp->SetBody(response.dump());
                    resp->content_type = APPLICATION_JSON;
                    return 404;
                }
                
            } 
            catch(const std::exception& e)
            {
                nlohmann::json response;
                response["error"] = "Bad credentials";
                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
                return 401;
            }
        }

        nlohmann::json response;
        response["error"] = "Not authentication!";
        resp->SetBody(response.dump());
        resp->content_type = APPLICATION_JSON;
        return 401;
    });

    router.Delete("/user/{userId}", [](HttpRequest *req, HttpResponse *resp)
    {
        try
        {
            auto basic_auth = req->GetHeader("Authorization");

            if (!basic_auth.empty()) {
                try {
                    nlohmann::json response;
                    int id;
                    
                    UserEntity authUserEntity = authenticationService.signin(basic_auth);
                    std::set<RoleEntity> rolesAuthUserEntity = authUserEntity.getRoles();
                    auto it = rolesAuthUserEntity.find(RoleEntity::ADMIN);
                    try {
                        std::stringstream ss(req->GetParam("userId"));
                        if (ss >> id) {
                            if(id < 0) {
                                response["error"] = "Id must be unsigned number";
                                resp->SetBody(response.dump());
                                resp->content_type = APPLICATION_JSON;
                                return 400;
                            }

                            if((unsigned int)id == authUserEntity.getId() || it != rolesAuthUserEntity.end()) {
                            
                                bool isDeleted = userService.deleteUser((unsigned int) id);

                                if(isDeleted) {
                                    response["status"] = "User with id = " + std::to_string(id) + " has been deleted!";
                                    resp->SetBody(response.dump());
                                    resp->content_type = APPLICATION_JSON;
                                    return 204;
                                } else {
                                    response["status"] = "User with id = " + std::to_string(id) + " has`t been deleted!";
                                    resp->SetBody(response.dump());
                                    resp->content_type = APPLICATION_JSON;
                                    return 400;
                                }
                            } else {
                                nlohmann::json response;
                                response["error"] = "Forbidden!";
                                resp->SetBody(response.dump());
                                resp->content_type = APPLICATION_JSON;
                                return 403;
                            }
                        } 
                    }
                    catch(const std::exception& e)
                    {
                        nlohmann::json response;
                        response["error"] = e.what();
                        resp->SetBody(response.dump());
                        resp->content_type = APPLICATION_JSON;
                        return 400;
                    }
                    
                } 
                catch(const std::exception& e)
                {
                    nlohmann::json response;
                    response["error"] = "Not authentication!";
                    resp->SetBody(response.dump());
                    resp->content_type = APPLICATION_JSON;
                    return 401;
                }
            }
        }
        catch(const std::exception& e)
        {
            nlohmann::json response;
            response["error"] = "Not authentication!";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 401;
        }
        
        nlohmann::json response;
        response["error"] = "Not authentication!";
        resp->SetBody(response.dump());
        resp->content_type = APPLICATION_JSON;
        return 401;
    });

    router.PATCH("/user/{userId}", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json request;


        auto basic_auth = req->GetHeader("Authorization");

        if (!basic_auth.empty())
        {
            try {
                UserEntity authUserEntity = authenticationService.signin(basic_auth);
                std::set<RoleEntity> rolesAuthUserEntity = authUserEntity.getRoles();
                auto it = rolesAuthUserEntity.find(RoleEntity::ADMIN);
                try {
                    nlohmann::json response;
                    request = nlohmann::json::parse(req->body);
                    int id;
                    std::stringstream ss(req->GetParam("userId"));
                    if (ss >> id) {
                        if(id < 0) {
                            response["error"] = "Id must be unsigned number";
                            resp->SetBody(response.dump());
                            resp->content_type = APPLICATION_JSON;
                            return 400;
                        }

                        if((unsigned int)id == authUserEntity.getId() || it != rolesAuthUserEntity.end()) {
                            try {
                                UserEntity user = userService.changeNameAndAgeById(id, request["name"], (unsigned int) request["age"]);
                                response["user"] = user.toJSON();
                                resp->SetBody(response.dump());
                                resp->content_type = APPLICATION_JSON;
                                return 200;
                            }
                            catch(const std::exception& e)
                            {
                                nlohmann::json response;
                                response["error"] = "Invalid JSON!";
                                resp->SetBody(response.dump());
                                resp->content_type = APPLICATION_JSON;
                                return 400;
                            }
                        } else {
                            nlohmann::json response;
                            response["error"] = "Forbidden!";
                            resp->SetBody(response.dump());
                            resp->content_type = APPLICATION_JSON;
                            return 403;
                        }

                    }
                }
                catch(const std::exception& e) {
                      nlohmann::json response;
                    response["error"] = e.what();
                    resp->SetBody(response.dump());
                    resp->content_type = APPLICATION_JSON;
                    return 400;
                }
            } 
            catch(const std::exception& e)
            {
                nlohmann::json response;
                response["error"] = "Not Authentication!";
                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
                return 401;
            }
        }
        nlohmann::json response;
        response["error"] = "Not authentication!";
        resp->SetBody(response.dump());
        resp->content_type = APPLICATION_JSON;
        return 401;
    });
}
#pragma once
#include <vector>
#include "user.h"

class UserManager {
private:
    std::vector<User> users;

public:
    void  addUser();
    void  printAllUsers();
    User* findUserByName(const std::string& name);
};

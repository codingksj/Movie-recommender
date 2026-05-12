#pragma once
#include <vector>
#include "user.h"

class UserManager {
private:
    std::vector<User> users;

public:
    void  addUser();
    void  printAllUsers();
    const User* findUserByName(const std::string& name) const;
    const std::vector<User>& getUsers() const { return users; }

    void loadUsers(const std::string& filename);
    void saveUsers(const std::string& filename);
};

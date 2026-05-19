#pragma once
#include <vector>
#include "user.h"
#include "BaseManager.h"

class UserManager : public BaseManager {
private:
    std::vector<User> users;

public:
    void  addUser();
    void  printAllUsers();
    const User* findUserByName(const std::string& name) const;
    const std::vector<User>& getUsers() const { return users; }

    void loadFromFile() override;
    void saveToFile() override;
    int size() override { return users.size(); }
};

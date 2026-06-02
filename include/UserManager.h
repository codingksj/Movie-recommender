#pragma once
#include <vector>
#include <string>
#include <utility>
#include "User.h"
#include "BaseManager.h"
#include "MovieConstant.h"

class UserManager : public BaseManager {
private:
    std::vector<User> users;

public:
    std::pair<UserResult, double> addUser(const std::string& name, const std::string& email);
    std::vector<std::string> getAllUsersFormatted() const;
    const User* findUserByName(const std::string& name) const;
    const std::vector<User>& getUsers() const { return users; }

    double loadFromFile() override;
    double saveToFile() override;
    int size() override { return users.size(); }
};

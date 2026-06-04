#pragma once
#include <vector>
#include <string>
#include <utility>
#include "User.h"
#include "BaseManager.h"
#include "MovieConstant.h"

using std::vector;
using std::string;
using std::pair;

class UserManager : public BaseManager {
private:
    vector<User> users;

public:
    pair<UserResult, double> addUser(const string& name, const string& email);
    vector<string> getAllUsersFormatted() const;
    const User* findUserByName(const string& name) const;
    const vector<User>& getUsers() const { return users; }

    double loadFromFile() override;
    double saveToFile() override;
    int size() override { return users.size(); }
};

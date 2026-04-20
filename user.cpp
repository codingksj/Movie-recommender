#include "user.h"
#include <iostream>

using std::string;
using std::cout;

User::User() : Base("", ""), userEmail("") {}
User::User(string userId, const string& userName, const string& userEmail)
    : Base("", userName) {
    
    this->id = Base::validateString(userId, "아이디");
    this->userEmail = Base::validateString(userEmail, "이메일");
}

string  User::getUserEmail()    const { return userEmail; }

void User::display() const {
    cout << "[ 유저 ID ] " << Base::id << " [ 유저 이름 ] " << Base::name << " [ 유저 이메일 ] " << userEmail << '\n';
}

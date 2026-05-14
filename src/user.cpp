#include "user.h"
#include <iostream>

using std::string;
using std::cout;

User::User(const string& userName, const string& userEmail)
    : Base(userName) {
    
    this->userEmail = Base::validateString(userEmail, "이메일");
}

string  User::getUserName()     const { return Base::name; }

string  User::getUserEmail()    const { return userEmail; }

bool User::operator==(const User& other) const {
    return name == other.name && userEmail == other.userEmail;
}

std::ostream& operator<<(std::ostream& os, const User& u) {
    os << "[ 유저 이름 ] " << u.name << " [ 유저 이메일 ] " << u.userEmail;
    return os;
}

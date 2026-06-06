#pragma once
#include "Base.h"

using std::ostream;

class User : public Base {
private:
    string      userEmail;

public:
    using Base::operator==;
    User(const string& name, const string& email);

    string      getUserName()       const;
    string      getUserEmail()      const;

    bool operator==(const User& other) const;

    friend ostream& operator<<(ostream& os, const User& u);
};

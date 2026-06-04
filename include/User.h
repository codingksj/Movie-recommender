#pragma once
#include "Base.h"

using std::ostream;

class User : public Base {
private:
    string      userEmail;

public:
    User(const string& userName, const string& userEmail);

    string      getUserName()       const;
    string      getUserEmail()      const;

    bool operator==(const User& other) const;

    friend ostream& operator<<(ostream& os, const User& u);
};

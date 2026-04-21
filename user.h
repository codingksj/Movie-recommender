#include "Base.h"

class User : public Base {
private:
    string      userEmail;

public:
    User(const string& userName, const string& userEmail);

    string      getUserName()       const;
    string      getUserEmail()      const;
    friend std::ostream& operator<<(std::ostream& os, const User& u);
};

#include "Base.h"

class User : public Base {
private:
    string      userEmail;

public:
    User();
    User(string userId, const string& userName, const string& userEmail);

    string      getUserId()         const { return Base::id; }
    string      getUserName()       const { return Base::name; }
    string      getUserEmail()      const;
    void        display()           const override;
};

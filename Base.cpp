#include "Base.h"
using std::string;
using std::cout;
Base::Base() : name("") {}
Base::Base(string name) : name(name) {}
Base::~Base() {}
string Base::getName() const { return name; }
string Base::validateString(const string& val, const string& fieldName) {
    if (val.length() > 50) {
        cout << fieldName << "이(가) 너무 깁니다!" << '\n';
        return val.substr(0, 50);
    }
    return val;
}

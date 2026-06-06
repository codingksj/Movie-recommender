// 공통 베이스 — 이름 필드, 문자열 검증·정규화

#include "Base.h"
#include "MovieConstant.h"

using namespace MC::Input;

using std::string;
using std::cout;

Base::Base() : name("") {}

Base::Base(string name) : name(name) {}

Base::~Base() {}

string Base::getName() const { return name; }

string Base::validateString(const string& val, const string& field) {
    if (val.length() > MAX_STR_LEN) {
        cout << field << "이(가) 너무 깁니다! (입력: " << val.length() << "자)" << '\n';
        return val.substr(0, MAX_STR_LEN);
    }
    return val;
}

string Base::normalizeString(const string& val) {
    string result = val;
    for (char& c : result) {
        c = tolower(c);
    }
    return result;
}

bool Base::operator==(const Base& other) const {
    return (*this) == other.name;
}

bool Base::operator==(const string& otherName) const {
    string lhs = normalizeString(name);
    string rhs = normalizeString(otherName);
    if (lhs.length() != rhs.length()) {
        return false;
    }
    for (size_t i = 0; i < lhs.length(); ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

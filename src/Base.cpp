// 공통 베이스 클래스: 이름 관리, 문자열 검증 및 정규화 제공

#include "Base.h"
#include "MovieConstant.h"

using std::string;
using std::cout;

// 기본 생성자/소멸자 및 문자열 유효성 검사
Base::Base() : name("") {}

Base::Base(string name) : name(name) {}

Base::~Base() {}

string Base::getName() const { return name; }

string Base::validateString(const string& val, const string& fieldName) {
    if (val.length() > MovieConstants::MAX_STR_LEN) {
        cout << fieldName << "이(가) 너무 깁니다! (입력: " << val.length() << "자)" << '\n';
        return val.substr(0, MovieConstants::MAX_STR_LEN);
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

// 대소문자 무시 비교 연산자
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

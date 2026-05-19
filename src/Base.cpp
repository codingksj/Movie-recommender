// 공통 데이터의 기준이 되는 Base 클래스

#include "Base.h"

using std::string;
using std::cout;

// 문자열 최대 유효 길이 상수
const size_t MAX_STR_LEN = 50;

// 기본 생성자
Base::Base() : name("") {}

// 이름을 받는 생성자
Base::Base(string name) : name(name) {}

// 가상 소멸자
Base::~Base() {}

// 이름 반환 함수
string Base::getName() const { return name; }

// 입력 문자열 길이 유효성 검사 함수
string Base::validateString(const string& val, const string& fieldName) {
    if (val.length() > MAX_STR_LEN) {
        cout << fieldName << "이(가) 너무 깁니다! (입력: " << val.length() << "자)" << '\n';
        return val.substr(0, MAX_STR_LEN);
    }
    return val;
}

// 대소문자 무시 동등 비교 연산자 (Base 객체 간)
bool Base::operator==(const Base& other) const {
    return (*this) == other.name;
}

// 대소문자 무시 동등 비교 연산자 (문자열 값과 비교)
bool Base::operator==(const string& otherName) const {
    if (name.length() != otherName.length()) {
        return false;
    }
    for (size_t i = 0; i < name.length(); ++i) {
        if (tolower(name[i]) != tolower(otherName[i])) {
            return false;
        }
    }
    return true;
}

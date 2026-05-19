// 사용자 정보를 담는 User 클래스

#include "user.h"
#include <iostream>

using std::string;
using std::cout;

// 이름과 이메일을 받는 생성자
User::User(const string& userName, const string& userEmail)
    : Base(userName) {
    this->userEmail = Base::validateString(userEmail, "이메일");
}

// 사용자 이름 반환
string  User::getUserName()     const { return Base::name; }

// 사용자 이메일 반환
string  User::getUserEmail()    const { return userEmail; }

// 동등 비교 연산자 오버로딩 (이름 및 이메일 기준)
bool User::operator==(const User& other) const {
    return name == other.name && userEmail == other.userEmail;
}

// 출력 스트림 연산자 오버로딩
std::ostream& operator<<(std::ostream& os, const User& u) {
    os << "[ 유저 이름 ] " << u.name << " [ 유저 이메일 ] " << u.userEmail;
    return os;
}

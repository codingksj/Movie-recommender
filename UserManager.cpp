#include "UserManager.h"
#include <iostream>

using std::cout;
using std::cin;
using std::string;

void UserManager::addUser() {
    string name, email;

    cout << "사용자 이름: ";
    cin.ignore();
    std::getline(cin, name);
    cout << "사용자 이메일: ";
    std::getline(cin, email);

    users.push_back(User(name, email));
    cout << "사용자가 추가되었습니다.\n";
}

void UserManager::printAllUsers() {
    if (users.empty()) {
        cout << "등록된 사용자가 없습니다.\n";
        return;
    }
    cout << "\n#### 전체 사용자 목록 ####\n";
    for (const auto& u : users) {
        cout << u << '\n';
    }
}

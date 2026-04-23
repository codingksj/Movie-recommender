#include "UserManager.h"
#include <iostream>

using std::cout;
using std::cin;
using std::string;
using std::vector;

void UserManager::addUser() {
    string name, email;

    cout << "사용자 이름: ";
    cin.ignore();
    if (!std::getline(cin, name) || name.empty()) {
        return;
    }

    if (findUserByName(name)) {
        cout << "이미 존재하는 사용자 이름입니다.\n";
        return;
    }

    cout << "사용자 이메일: ";
    if (!std::getline(cin, email) || email.empty()) {
        return;
    }

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

User* UserManager::findUserByName(const string& name) {
    for (auto& u : users) {
        if (u.getUserName() == name) {
            return &u;
        }
    }

    return nullptr;
}

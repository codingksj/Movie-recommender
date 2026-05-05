#include "UserManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

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

    cout << "사용자 이메일: ";
    if (!std::getline(cin, email) || email.empty()) {
        return;
    }

    User newUser(name, email);
    for (const auto& u : users) {
        if (u == newUser) {
            cout << "[" << name << ", " << email << "]은(는) 이미 존재하는 사용자입니다.\n";
            return;
        }
    }

    users.push_back(newUser);
    cout << "사용자가 추가되었습니다. [이름: " << name << ", 이메일: " << email << "]\n";
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

void UserManager::loadUsers(const string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "파일을 열 수 없습니다: " << filename << "\n";
        return;
    }

    string line;
    std::getline(file, line); // 헤더 스킵 (userName,userEmail)
    
    int count = 0;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        string name, email;

        std::getline(ss, name, ',');
        std::getline(ss, email, ',');

        if (!name.empty()) {
            users.push_back(User(name, email));
            count++;
        }
    }
    file.close();
    cout << "사용자 데이터 로드 완료 (" << count << "건)\n";
}

void UserManager::saveUsers(const string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        cout << "파일을 저장할 수 없습니다: " << filename << "\n";
        return;
    }

    file << "userName,userEmail\n";
    for (const auto& u : users) {
        file << u.getUserName() << "," << u.getUserEmail() << "\n";
    }
    file.close();
    cout << "사용자 데이터 저장 완료\n";
}

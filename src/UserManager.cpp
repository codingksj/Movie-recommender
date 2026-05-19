// 사용자 추가 및 관리를 담당하는 클래스

#include "UserManager.h"
#include "menu.h"
#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::cin;
using std::string;
using std::vector;

// 신규 사용자 추가 프롬프트 처리
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

// 등록된 전체 사용자 목록 출력
void UserManager::printAllUsers() {
    if (users.empty()) {
        cout << "등록된 사용자가 없습니다.\n";
        return;
    }

    vector<string> lines;
    for (const auto& u : users) {
        std::stringstream ss;
        ss << " " << u;
        lines.push_back(ss.str());
    }
    Menu::showDynamicResult("전체 사용자 목록", lines);
}

// 이름으로 사용자 포인터 찾기
const User* UserManager::findUserByName(const string& name) const {
    for (const auto& u : users) {
        if (u.getUserName() == name) {
            return &u;
        }
    }

    return nullptr;
}

// 파일에서 사용자 데이터 불러오기
void UserManager::loadFromFile() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        cout << "파일을 열 수 없습니다: " << filePath << "\n";
        return;
    }

    string line;
    std::getline(file, line);
    
    int count = 0;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
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
}

// 파일로 사용자 데이터 저장하기
void UserManager::saveToFile() {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        cout << "파일을 저장할 수 없습니다: " << filePath << "\n";
        return;
    }

    file << "userName,userEmail\n";
    for (const auto& u : users) {
        file << u.getUserName() << "," << u.getUserEmail() << "\n";
    }
    file.close();
}

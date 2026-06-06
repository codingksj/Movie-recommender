// 사용자 CRUD 및 CSV 영속화

#include "UserManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;
using Clock = chrono::high_resolution_clock;
using Ms = chrono::microseconds;

pair<UserResult, double> UserManager::addUser(const string& name, const string& email) {
    auto start = Clock::now();

    User newUser(name, email);
    for (const auto& u : users) {
        if (u == newUser) {
            auto end = Clock::now();
            double ms = chrono::duration_cast<Ms>(end - start).count() / 1000.0;
            return {UserResult::DUPLICATE_USER, ms};
        }
    }

    users.push_back(newUser);

    auto end = Clock::now();
    double ms = chrono::duration_cast<Ms>(end - start).count() / 1000.0;

    return {UserResult::SUCCESS, ms};
}

vector<string> UserManager::getAllUsersFormatted() const {
    vector<string> lines;
    for (const auto& u : users) {
        stringstream ss;
        ss << " " << u;
        lines.push_back(ss.str());
    }
    return lines;
}

const User* UserManager::findUserByName(const string& name) const {
    string searchName = Base::normalizeString(name);

    for (const auto& u : users) {
        if (Base::normalizeString(u.getUserName()) == searchName) {
            return &u;
        }
    }
    return nullptr;
}

double UserManager::loadFromFile() {
    users.clear();
    auto start = Clock::now();
    int count = 0;
    try {
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "파일을 열 수 없습니다: " << filePath << "\n";
            return 0.0;
        }
        string line;
        getline(file, line);
        while (getline(file, line)) {
            if (line.empty()) { continue; }
            stringstream ss(line);
            string name, email;
            getline(ss, name, ',');
            getline(ss, email, ',');
            if (!name.empty()) {
                users.push_back(User(name, email));
                count++;
            }
        }
        file.close();
    } catch (const exception& e) {
        cerr << "UserManager::loadFromFile 예외 발생: " << e.what() << "\n";
    }
    auto end = Clock::now();
    return chrono::duration_cast<Ms>(end - start).count() / 1000.0;
}

double UserManager::saveToFile() {
    auto start = Clock::now();
    try {
        ofstream file(filePath);
        if (!file.is_open()) {
            cerr << "파일을 저장할 수 없습니다: " << filePath << "\n";
            return 0.0;
        }
        file << "userName,userEmail\n";
        for (const auto &u : users) {
            file << u.getUserName() << "," << u.getUserEmail() << "\n";
        }
        file.close();
    } catch (const exception &e) {
        cerr << "UserManager::saveToFile 예외 발생: " << e.what() << "\n";
    }
    auto end = Clock::now();
    return chrono::duration_cast<Ms>(end - start).count() / 1000.0;
}

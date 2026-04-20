#pragma once
#include <string>
#include <iostream>

using std::string;
using std::cout;

class Base {
protected:
    string id;
    string name;

public:
    Base() : id(""), name("") {}
    Base(string id, string name) : id(id), name(name) {}
    virtual ~Base() {}

    virtual void display() const = 0;

    string getId() const { return id; }
    string getName() const { return name; }

    static string validateString(const string& val, const string& fieldName) {
        if (val.length() > 50) {
            cout << fieldName << "이(가) 너무 깁니다!" << '\n';
            return val.substr(0, 50);
        }
        return val;
    }
};

#pragma once
#include <string>
#include <iostream>

using std::string;
using std::cout;

class Base {
protected:
    string name;

public:
    Base();
    Base(string name);
    virtual ~Base();

    string getName() const;

    static string validateString(const string& val, const string& fieldName);
    static string normalizeString(const string& val);
    virtual bool operator==(const Base& other) const;
    virtual bool operator==(const string& otherName) const;
};

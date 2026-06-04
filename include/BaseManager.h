#pragma once
#include <string>

using std::string;

class BaseManager {
protected:
    string filePath;

public:
    virtual ~BaseManager() = default;

    void setFilePath(const string& path) { filePath = path; }
    const string& getFilePath() const { return filePath; }

    virtual double loadFromFile() = 0;
    virtual double saveToFile() = 0;
    virtual int size() = 0;
};

#pragma once
#include <string>

class BaseManager {
protected:
    std::string filePath;

public:
    virtual ~BaseManager() = default;

    void setFilePath(const std::string& path) { filePath = path; }
    const std::string& getFilePath() const { return filePath; }

    virtual double loadFromFile() = 0;
    virtual double saveToFile() = 0;
    virtual int size() = 0;
};

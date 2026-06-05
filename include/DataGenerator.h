#pragma once

#include <string>

class DataGenerator {
public:
    static bool generateSyntheticDataset(const std::string& movieOutputPath,
                                         const std::string& userOutputPath,
                                         const std::string& ratingOutputPath,
                                         size_t movieCount,
                                         size_t userCount,
                                         size_t ratingCount,
                                         int okPercent = 0);
};

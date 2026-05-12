#pragma once

#include "MovieManager.h"
#include "RatingManager.h"
#include "UserManager.h"
#include <string>
#include <vector>

class Recommend {
private:
    const MovieManager& movieManager;
    const RatingManager& ratingManager;
    const UserManager& userManager;

    // 유사도 계산 (기존 SimilarityCalculator의 로직을 가져옴)
    double calculateSimilarity(const std::string& userName1, const std::string& userName2) const;

public:
    Recommend(const MovieManager& mm, const RatingManager& rm, const UserManager& um)
        : movieManager(mm), ratingManager(rm), userManager(um) {}

    void recommend();
    void findSimilarUsers();
};

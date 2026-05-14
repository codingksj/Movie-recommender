#pragma once

#include "MovieManager.h"
#include "RatingManager.h"
#include "UserManager.h"
#include <string>
#include <vector>

class Recommender {
private:
    const MovieManager& movieManager;
    const RatingManager& ratingManager;
    const UserManager& userManager;


    double calculateSimilarity(const std::vector<Rating>& ratingsA,
                               const std::vector<Rating>& ratingsB) const;

    std::vector<Rating> getUserRatings(const std::string& userName) const;

public:
    Recommender(const MovieManager& mm, const RatingManager& rm, const UserManager& um)
        : movieManager(mm), ratingManager(rm), userManager(um) {}

    std::vector<std::string> recommend(const std::string& targetUser, int K, int N);
    void findSimilarUsers();
    void recommendMenu();
};

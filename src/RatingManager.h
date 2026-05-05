#pragma once
#include <vector>
#include <string>
#include "rating.h"
#include "MovieManager.h"
#include "UserManager.h"

class RatingManager {
private:
    std::vector<Rating> ratings;

public:
    void addRating(MovieManager& movieManager, UserManager& userManager);
    void printRatingsByMovie();

    void loadRatings(const std::string& filename);
    void saveRatings(const std::string& filename);
};

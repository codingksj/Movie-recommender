#pragma once
#include <vector>
#include <string>
#include "rating.h"
#include "MovieManager.h"

class RatingManager {
private:
    std::vector<Rating> ratings;

public:
    void addRating(MovieManager& movieManager);
    void printRatingsByMovie();
};

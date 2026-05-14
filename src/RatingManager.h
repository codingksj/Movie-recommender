#pragma once
#include <vector>
#include <string>
#include "rating.h"
#include "MovieManager.h"
#include "UserManager.h"
#include "BaseManager.h"

class RatingManager : public BaseManager {
private:
    std::vector<Rating> ratings;

public:
    void addRating(MovieManager& movieManager, UserManager& userManager);
    void printRatingsByMovie();
    const std::vector<Rating>& getRatings() const { return ratings; }

    void loadFromFile() override;
    void saveToFile() override;
    int size() override { return ratings.size(); }
};

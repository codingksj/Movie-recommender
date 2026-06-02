#pragma once
#include <vector>
#include <string>
#include <utility>
#include "Rating.h"
#include "MovieManager.h"
#include "UserManager.h"
#include "BaseManager.h"
#include "MovieConstant.h"

class RatingManager : public BaseManager {
private:
    std::vector<Rating> ratings;

public:
    std::pair<RatingResult, double> addRating(const std::string& userName, const std::string& movieTitle, double score, MovieManager& movieManager, UserManager& userManager);
    std::vector<std::string> getRatingsByMovieFormatted(const std::string& movieTitle) const;
    const std::vector<Rating>& getRatings() const { return ratings; }

    double loadFromFile() override;
    double saveToFile() override;
    int size() override { return ratings.size(); }
};

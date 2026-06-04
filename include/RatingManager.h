#pragma once
#include <vector>
#include <string>
#include <utility>
#include "Rating.h"
#include "MovieManager.h"
#include "UserManager.h"
#include "BaseManager.h"
#include "MovieConstant.h"

using std::vector;
using std::string;
using std::pair;

class RatingManager : public BaseManager {
private:
    vector<Rating> ratings;

public:
    pair<RatingResult, double> addRating(const string& userName, const string& movieTitle, double score, MovieManager& movieManager, UserManager& userManager);
    vector<string> getRatingsByMovieFormatted(const string& movieTitle) const;
    const vector<Rating>& getRatings() const { return ratings; }

    double loadFromFile() override;
    double saveToFile() override;
    int size() const override { return ratings.size(); }
};

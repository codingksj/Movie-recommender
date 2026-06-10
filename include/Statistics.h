#pragma once

#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include "MovieConstant.h"
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;
using std::pair;

class Statistics {
public:
    using StatPair = pair<double, int>;
    using RatingCountPair = pair<int, double>;
    
    using GenreMap = map<string, StatPair>;
    using YearMap = map<int, StatPair>;
    using UserMap = map<string, RatingCountPair>;

private:
    const MovieManager& mm;
    const RatingManager& rm;
    const UserManager& um;

public:
    Statistics(const MovieManager& mm,
               const RatingManager& rm,
               const UserManager& um)
        : mm(mm), rm(rm), um(um) {}

    vector<string> overall() const;
    vector<string> topByRating(int n = MC::Ui::TOP_N) const;
    vector<string> topByRatingCount(int n = MC::Ui::TOP_N) const;
    vector<string> byGenre() const;
    vector<string> byYear() const;
    vector<string> topUsers(int n = MC::Ui::TOP_N) const;
    vector<string> userStats(const string& user) const;

private:
    double avgRating() const;
    GenreMap genreStats() const;
    YearMap yearStats() const;
    UserMap userRatingStats() const;
    
    vector<Rating> userRatings(const string& user) const;
    vector<string> formatUserLines(const string& user,
                                   const vector<Rating>& ratings) const;
};

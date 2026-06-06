#pragma once

#include "MovieManager.h"
#include "RatingManager.h"
#include "UserManager.h"
#include <string>
#include <vector>
#include <map>
#include <set>

using std::string;
using std::vector;
using std::map;
using std::set;
using std::pair;

class Recommender {
private:
    const MovieManager& mm;
    const RatingManager& rm;
    const UserManager& um;

    double calculateSimilarity(const vector<Rating>& a, const vector<Rating>& b) const;

    map<string, double> predictMovieScores(const vector<pair<string, double>>& userSims,
                                           const set<string>& watched, size_t k);

    double calculateGenreSimilarity(const Movie& m1, const Movie& m2) const;
    std::vector<Movie> filterByGenre(const string& genre) const;

public:
    Recommender(const MovieManager& mm, const RatingManager& rm, const UserManager& um)
        : mm(mm), rm(rm), um(um) {}

    const UserManager& getUserManager() const { return um; }
    vector<Rating> getUserRatings(const string& user) const;
    vector<pair<string, double>> getSimilarUsers(const string& user, int limit) const;

    vector<string> recommend(const string& user, int k, int n, const string& genre = "");
    vector<string> recommendByGenre(const string& title, int n) const;
};

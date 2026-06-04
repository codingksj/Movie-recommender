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
    const MovieManager& movieManager;
    const RatingManager& ratingManager;
    const UserManager& userManager;

    double calculateSimilarity(const vector<Rating>& ratingsA,
                               const vector<Rating>& ratingsB) const;

    map<string, double> predictMovieScores(const vector<pair<string, double>>& userSims, 
                                                     const set<string>& watchedMovies, size_t actualK);

    double calculateGenreSimilarity(const Movie& m1, const Movie& m2) const;

public:
    Recommender(const MovieManager& mm, const RatingManager& rm, const UserManager& um)
        : movieManager(mm), ratingManager(rm), userManager(um) {}

    const UserManager& getUserManager() const { return userManager; }
    vector<Rating> getUserRatings(const string& userName) const;
    vector<pair<string, double>> getSimilarUsers(const string& targetUser, int limit) const;

    vector<string> recommend(const string& targetUser, int K, int N);
    vector<string> recommendByGenre(const string& targetMovieTitle, int N) const;
};

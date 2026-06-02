#pragma once

#include "MovieManager.h"
#include "RatingManager.h"
#include "UserManager.h"
#include <string>
#include <vector>
#include <map>
#include <set>

class Recommender {
private:
    const MovieManager& movieManager;
    const RatingManager& ratingManager;
    const UserManager& userManager;

    double calculateSimilarity(const std::vector<Rating>& ratingsA,
                               const std::vector<Rating>& ratingsB) const;

    std::map<std::string, double> predictMovieScores(const std::vector<std::pair<std::string, double>>& userSims, 
                                                     const std::set<std::string>& watchedMovies, size_t actualK);

    double calculateGenreSimilarity(const Movie& m1, const Movie& m2) const;

public:
    Recommender(const MovieManager& mm, const RatingManager& rm, const UserManager& um)
        : movieManager(mm), ratingManager(rm), userManager(um) {}

    const UserManager& getUserManager() const { return userManager; }
    std::vector<Rating> getUserRatings(const std::string& userName) const;
    std::vector<std::pair<std::string, double>> getSimilarUsers(const std::string& targetUser, int limit) const;

    std::vector<std::string> recommend(const std::string& targetUser, int K, int N);
    std::vector<std::string> recommendByGenre(const std::string& targetMovieTitle, int N) const;
};

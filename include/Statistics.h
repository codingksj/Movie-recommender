#pragma once

#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;
using std::pair;

// 영화·사용자·평점 통계 집계
class Statistics {
private:
    const MovieManager& mm;
    const RatingManager& rm;
    const UserManager& um;

public:
    Statistics(const MovieManager& mm, const RatingManager& rm, const UserManager& um)
        : mm(mm), rm(rm), um(um) {}

    vector<string> getOverallStatistics() const;
    vector<string> getTopMoviesByRating(int n = 10) const;
    vector<string> getTopMoviesByRatingCount(int n = 10) const;
    vector<string> getStatisticsByGenre() const;
    vector<string> getStatisticsByYear() const;
    vector<string> getTopUsersByRatingCount(int n = 10) const;
    vector<string> getUserStatistics(const string& user) const;

private:
    double calculateAverageRating() const;
    map<string, pair<double, int>> getGenreStatistics() const;
    map<int, pair<double, int>> getYearStatistics() const;
    map<string, pair<int, double>> getUserRatingStatistics() const;
    
    // 헬퍼 함수 - getUserStatistics 분해 (함수 복잡도 감소 목적)
    vector<Rating> getUserRatingsVector(const string& user) const;
    vector<string> formatUserStatisticsLines(const string& user, const vector<Rating>& ratings) const;
};

// 영화·사용자·평점 통계 집계

#include "Statistics.h"
#include "MovieConstant.h"
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace MC::Ui;

vector<string> Statistics::getOverallStatistics() const {
    vector<string> result;
    
    int movieCount = mm.size();
    int userCount = um.size();
    int ratingCount = rm.size();
    double avgRating = calculateAverageRating();
    
    stringstream ss;
    ss << "  전체 영화 수     : " << movieCount << "개";
    result.push_back(ss.str());
    
    ss.str(""); ss.clear();
    ss << "  전체 사용자 수   : " << userCount << "명";
    result.push_back(ss.str());
    
    ss.str(""); ss.clear();
    ss << "  전체 평점 데이터 : " << ratingCount << "건";
    result.push_back(ss.str());
    
    ss.str(""); ss.clear();
    ss << "  전체 평균 평점   : " << fixed << setprecision(2) << avgRating;
    result.push_back(ss.str());
    
    return result;
}

vector<string> Statistics::getTopMoviesByRating(int n) const {
    const auto& movies = mm.getMovies();
    if (movies.empty()) { return {"  추천할 영화 데이터가 없습니다."}; }
    
    auto sorted = movies;
    sort(sorted.begin(), sorted.end(), [](const Movie& a, const Movie& b) {
        if (a.getAverageRating() != b.getAverageRating()) {
            return a.getAverageRating() > b.getAverageRating();
        }
        return a.getTitle() < b.getTitle();
    });
    
    if (n <= 0) {
        return {};
    }
    size_t limit = n < 0 ? 0 : n;
    if (limit > sorted.size()) {
        limit = sorted.size();
    }
    
    vector<string> result;
    for (size_t i = 0; i < limit; ++i) {
        stringstream ss;
        ss << "    " << setw(FORMAT_TITLE_WIDTH) << left << sorted[i].getTitle()
           << " | 평점: " << fixed << setprecision(1) << setw(FORMAT_SCORE_WIDTH) << right << sorted[i].getAverageRating()
           << " | 평가: " << setw(FORMAT_RATING_COUNT_WIDTH) << sorted[i].getRatingCount();
        result.push_back(ss.str());
    }
    return result;
}

vector<string> Statistics::getTopMoviesByRatingCount(int n) const {
    const auto& movies = mm.getMovies();
    if (movies.empty()) { return {"  추천할 영화 데이터가 없습니다."}; }
    
    auto sorted = movies;
    sort(sorted.begin(), sorted.end(), [](const Movie& a, const Movie& b) {
        if (a.getRatingCount() != b.getRatingCount()) {
            return a.getRatingCount() > b.getRatingCount();
        }
        if (a.getAverageRating() != b.getAverageRating()) {
            return a.getAverageRating() > b.getAverageRating();
        }
        return a.getTitle() < b.getTitle();
    });
    
    if (n <= 0) {
        return {};
    }
    size_t limit = n < 0 ? 0 : n;
    if (limit > sorted.size()) {
        limit = sorted.size();
    }
    
    vector<string> result;
    for (size_t i = 0; i < limit; ++i) {
        stringstream ss;
        ss << "    " << setw(FORMAT_TITLE_WIDTH) << left << sorted[i].getTitle()
           << " | 평가: " << setw(FORMAT_RATING_COUNT_WIDTH) << right << sorted[i].getRatingCount()
           << " | 평점: " << fixed << setprecision(1) << setw(FORMAT_SCORE_WIDTH) << sorted[i].getAverageRating();
        result.push_back(ss.str());
    }
    return result;
}

vector<string> Statistics::getStatisticsByGenre() const {
    auto genreStats = getGenreStatistics();
    if (genreStats.empty()) { return {"  장르 데이터가 없습니다."}; }
    
    vector<string> result;
    for (const auto& [genre, statPair] : genreStats) {
        double avg = statPair.first;
        int count = statPair.second;
        stringstream ss;
        ss << "  " << setw(FORMAT_GENRE_WIDTH) << left << genre << " | 평균: " << fixed << setprecision(1) << setw(FORMAT_SCORE_WIDTH) << right << avg
           << " | 영화 수: " << setw(FORMAT_COUNT_WIDTH) << count;
        result.push_back(ss.str());
    }
    return result;
}

vector<string> Statistics::getStatisticsByYear() const {
    auto yearStats = getYearStatistics();
    if (yearStats.empty()) { return {"  연도 데이터가 없습니다."}; }
    
    vector<string> result;
    for (const auto& [year, statPair] : yearStats) {
        double avg = statPair.first;
        int count = statPair.second;
        stringstream ss;
        ss << "  " << setw(FORMAT_YEAR_WIDTH) << year << "년 | 평균: " << fixed << setprecision(1) << setw(FORMAT_SCORE_WIDTH) << right << avg
           << " | 영화 수: " << setw(FORMAT_COUNT_WIDTH) << count;
        result.push_back(ss.str());
    }
    return result;
}

vector<string> Statistics::getTopUsersByRatingCount(int n) const {
    auto userStats = getUserRatingStatistics();
    if (userStats.empty()) { return {"  사용자 평점 데이터가 없습니다."}; }
    
    vector<pair<string, pair<int, double>>> sorted(userStats.begin(), userStats.end());
    sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        if (a.second.first != b.second.first) {
            return a.second.first > b.second.first;
        }
        if (a.second.second != b.second.second) {
            return a.second.second > b.second.second;
        }
        return a.first < b.first;
    });
    
    if (n <= 0) {
        return {};
    }
    size_t limit = n < 0 ? 0 : n;
    if (limit > sorted.size()) {
        limit = sorted.size();
    }
    
    vector<string> result;
    for (size_t i = 0; i < limit; ++i) {
        const auto& [userName, statPair] = sorted[i];
        int count = statPair.first;
        double avg = statPair.second;
        stringstream ss;
        ss << "    " << setw(FORMAT_USER_WIDTH) << left << userName
           << " | 평가: " << setw(FORMAT_RATING_COUNT_WIDTH) << right << count
           << " | 평균: " << fixed << setprecision(2) << setw(FORMAT_AVG_WIDTH) << avg;
        result.push_back(ss.str());
    }
    return result;
}

vector<string> Statistics::getUserStatistics(const string& user) const {
    vector<string> result;

    const User* found = um.findUserByName(user);
    if (!found) {
        result.push_back("  존재하지 않는 사용자입니다.");
        return result;
    }

    // 사용자의 평점 벡터 수집 - 모든 평점에서 해당 사용자 평점만 추출 (분해된 함수 호출)
    vector<Rating> userRatings = getUserRatingsVector(user);
    
    // 수집된 평점을 기반으로 통계 포맷팅 (분해된 함수 호출)
    return formatUserStatisticsLines(user, userRatings);
}

double Statistics::calculateAverageRating() const {
    const auto& ratings = rm.getRatings();
    if (ratings.empty()) { return 0.0; }
    
    double sum = accumulate(ratings.begin(), ratings.end(), 0.0,
        [](double acc, const Rating& r) {
            return acc + r.getUserRating();
        });
    return sum / ratings.size();
}

map<string, pair<double, int>> Statistics::getGenreStatistics() const {
    map<string, pair<double, int>> genreStats; 
    
    const auto& movies = mm.getMovies();
    for (const auto& movie : movies) {
        string genre = movie.getGenre();
        if (genreStats.find(genre) == genreStats.end()) {
            genreStats[genre] = {0.0, 0};
        }
        genreStats[genre].first += movie.getAverageRating() * movie.getRatingCount();
        genreStats[genre].second += movie.getRatingCount();
    }
    
    map<string, pair<double, int>> result;
    for (const auto& [genre, statPair] : genreStats) {
        double totalRating = statPair.first;
        int count = statPair.second;
        if (count > 0) {
            result[genre] = {totalRating / count, count};
        }
    }
    return result;
}

map<int, pair<double, int>> Statistics::getYearStatistics() const {
    map<int, pair<double, int>> yearStats; 
    
    const auto& movies = mm.getMovies();
    for (const auto& movie : movies) {
        int year = movie.getReleaseYear();
        if (yearStats.find(year) == yearStats.end()) {
            yearStats[year] = {0.0, 0};
        }
        yearStats[year].first += movie.getAverageRating() * movie.getRatingCount();
        yearStats[year].second += movie.getRatingCount();
    }
    
    map<int, pair<double, int>> result;
    for (const auto& [year, statPair] : yearStats) {
        double totalRating = statPair.first;
        int count = statPair.second;
        if (count > 0) {
            result[year] = {totalRating / count, count};
        }
    }
    return result;
}

map<string, pair<int, double>> Statistics::getUserRatingStatistics() const {
    map<string, pair<int, double>> userStats; 
    
    const auto& ratings = rm.getRatings();
    for (const auto& rating : ratings) {
        string userName = rating.getUserName();
        if (userStats.find(userName) == userStats.end()) {
            userStats[userName] = {0, 0.0};
        }
        userStats[userName].first++;
        userStats[userName].second += rating.getUserRating();
    }
    
    // 사용자별 평균 평점을 계산하여 통계 맵에 저장
    map<string, pair<int, double>> result;
    for (const auto& [userName, statPair] : userStats) {
        int count = statPair.first;
        double totalRating = statPair.second;
        result[userName] = {count, count > 0 ? totalRating / count : 0.0};
    }
    return result;
}

// 사용자 평점 벡터 수집 - 모든 평점 데이터에서 특정 사용자의 평점만 추출
vector<Rating> Statistics::getUserRatingsVector(const string& user) const {
    vector<Rating> userRatings;
    const User* found = um.findUserByName(user);
    if (!found) {
        return userRatings;
    }
    
    const auto& allRatings = rm.getRatings();
    for (const auto& r : allRatings) {
        if (r.getUserName() == found->getUserName()) {
            userRatings.push_back(r);
        }
    }
    return userRatings;
}

// 사용자 통계 포맷팅 - 수집된 평점들을 기반으로 통계 정보를 텍스트 형식으로 변환
vector<string> Statistics::formatUserStatisticsLines(const string& user, const vector<Rating>& userRatings) const {
    vector<string> result;
    
    if (userRatings.empty()) {
        result.push_back("  해당 사용자의 평점 이력이 없습니다.");
        return result;
    }
    
    int ratingCount = userRatings.size();
    double sumRating = accumulate(userRatings.begin(), userRatings.end(), 0.0,
        [](double acc, const Rating& r) {
            return acc + r.getUserRating();
        });
    double avgRating = sumRating / ratingCount;
    
    stringstream ss;
    ss << "  사용자명       : " << user;
    result.push_back(ss.str());
    
    ss.str(""); ss.clear();
    ss << "  평가한 영화 수 : " << ratingCount << "개";
    result.push_back(ss.str());
    
    ss.str(""); ss.clear();
    ss << "  평균 평점     : " << fixed << setprecision(2) << avgRating;
    result.push_back(ss.str());
    
    ss.str(""); ss.clear();
    ss << "  최고 평점     : " << fixed << setprecision(1) 
       << (*max_element(userRatings.begin(), userRatings.end(),
           [](const Rating& a, const Rating& b) {
               return a.getUserRating() < b.getUserRating();
           })).getUserRating();
    result.push_back(ss.str());
    
    ss.str(""); ss.clear();
    ss << "  최저 평점     : " << fixed << setprecision(1)
       << (*min_element(userRatings.begin(), userRatings.end(),
           [](const Rating& a, const Rating& b) {
               return a.getUserRating() < b.getUserRating();
           })).getUserRating();
    result.push_back(ss.str());
    
    return result;
}

#include "Statistics.h"
#include "MovieConstant.h"
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace MC::Ui;

vector<string> Statistics::overall() const {
    vector<string> result;
    
    int movieCount = mm.size();
    int userCount = um.size();
    int ratingCount = rm.size();
    double avg = avgRating();
    
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
    ss << "  전체 평균 평점   : " << fixed << setprecision(2) << avg;
    result.push_back(ss.str());
    
    return result;
}

vector<string> Statistics::topByRating(int n) const {
    const auto& movies = mm.getMovies();
    if (movies.empty()) {
        return {"  추천할 영화 데이터가 없습니다."};
    }
    
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
    size_t limit = static_cast<size_t>(n);
    if (limit > sorted.size()) {
        limit = sorted.size();
    }
    
    vector<string> result;
    for (size_t i = 0; i < limit; ++i) {
        stringstream ss;
        ss << "    " << setw(FORMAT_TITLE_WIDTH) << left << sorted[i].getTitle()
           << " | 평점: " << fixed << setprecision(1) << setw(FORMAT_SCORE_WIDTH) 
           << right << sorted[i].getAverageRating()
           << " | 평가: " << setw(FORMAT_RATING_COUNT_WIDTH) 
           << sorted[i].getRatingCount();
        result.push_back(ss.str());
    }
    return result;
}

vector<string> Statistics::topByRatingCount(int n) const {
    const auto& movies = mm.getMovies();
    if (movies.empty()) {
        return {"  추천할 영화 데이터가 없습니다."};
    }
    
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
    size_t limit = static_cast<size_t>(n);
    if (limit > sorted.size()) {
        limit = sorted.size();
    }
    
    vector<string> result;
    for (size_t i = 0; i < limit; ++i) {
        stringstream ss;
        ss << "    " << setw(FORMAT_TITLE_WIDTH) << left << sorted[i].getTitle()
           << " | 평가: " << setw(FORMAT_RATING_COUNT_WIDTH) << right 
           << sorted[i].getRatingCount()
           << " | 평점: " << fixed << setprecision(1) << setw(FORMAT_SCORE_WIDTH) 
           << sorted[i].getAverageRating();
        result.push_back(ss.str());
    }
    return result;
}

vector<string> Statistics::byGenre() const {
    auto stats = genreStats();
    if (stats.empty()) {
        return {"  장르 데이터가 없습니다."};
    }
    
    vector<string> result;
    for (const auto& [genre, statPair] : stats) {
        double avg = statPair.first;
        int count = statPair.second;
        stringstream ss;
        ss << "  " << setw(FORMAT_GENRE_WIDTH) << left << genre << " | 평균: " 
           << fixed << setprecision(1) << setw(FORMAT_SCORE_WIDTH) << right << avg
           << " | 영화 수: " << setw(FORMAT_COUNT_WIDTH) << count;
        result.push_back(ss.str());
    }
    return result;
}

vector<string> Statistics::byYear() const {
    auto stats = yearStats();
    if (stats.empty()) {
        return {"  연도 데이터가 없습니다."};
    }
    
    vector<string> result;
    for (const auto& [year, statPair] : stats) {
        double avg = statPair.first;
        int count = statPair.second;
        stringstream ss;
        ss << "  " << setw(FORMAT_YEAR_WIDTH) << year << "년 | 평균: " << fixed 
           << setprecision(1) << setw(FORMAT_SCORE_WIDTH) << right << avg
           << " | 영화 수: " << setw(FORMAT_COUNT_WIDTH) << count;
        result.push_back(ss.str());
    }
    return result;
}

vector<string> Statistics::topUsers(int n) const {
    auto stats = userRatingStats();
    if (stats.empty()) {
        return {"  사용자 평점 데이터가 없습니다."};
    }
    
    vector<pair<string, RatingCountPair>> sorted(stats.begin(), stats.end());
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
    size_t limit = static_cast<size_t>(n);
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

vector<string> Statistics::userStats(const string& user) const {
    vector<string> result;

    const User* found = um.findUserByName(user);
    if (!found) {
        result.push_back("  존재하지 않는 사용자입니다.");
        return result;
    }

    vector<Rating> ratings = userRatings(user);
    return formatUserLines(user, ratings);
}

double Statistics::avgRating() const {
    const auto& ratings = rm.getRatings();
    if (ratings.empty()) {
        return 0.0;
    }
    
    double sum = accumulate(ratings.begin(), ratings.end(), 0.0,
        [](double acc, const Rating& r) {
            return acc + r.getUserRating();
        });
    return sum / ratings.size();
}

Statistics::GenreMap Statistics::genreStats() const {
    map<string, StatPair> stats; 
    
    const auto& movies = mm.getMovies();
    for (const auto& movie : movies) {
        string g = movie.getGenre();
        if (stats.find(g) == stats.end()) {
            stats[g] = {0.0, 0};
        }
        stats[g].first += movie.getAverageRating() * movie.getRatingCount();
        stats[g].second += movie.getRatingCount();
    }
    
    GenreMap result;
    for (const auto& [g, statPair] : stats) {
        double totalRating = statPair.first;
        int count = statPair.second;
        if (count > 0) {
            result[g] = {totalRating / count, count};
        }
    }
    return result;
}

Statistics::YearMap Statistics::yearStats() const {
    map<int, StatPair> stats; 
    
    const auto& movies = mm.getMovies();
    for (const auto& movie : movies) {
        int year = movie.getReleaseYear();
        if (stats.find(year) == stats.end()) {
            stats[year] = {0.0, 0};
        }
        stats[year].first += movie.getAverageRating() * movie.getRatingCount();
        stats[year].second += movie.getRatingCount();
    }
    
    YearMap result;
    for (const auto& [year, statPair] : stats) {
        double totalRating = statPair.first;
        int count = statPair.second;
        if (count > 0) {
            result[year] = {totalRating / count, count};
        }
    }
    return result;
}

Statistics::UserMap Statistics::userRatingStats() const {
    map<string, RatingCountPair> stats; 
    
    const auto& ratings = rm.getRatings();
    for (const auto& rating : ratings) {
        string userName = rating.getUserName();
        if (stats.find(userName) == stats.end()) {
            stats[userName] = {0, 0.0};
        }
        stats[userName].first++;
        stats[userName].second += rating.getUserRating();
    }
    
    UserMap result;
    for (const auto& [userName, statPair] : stats) {
        int count = statPair.first;
        double totalRating = statPair.second;
        result[userName] = {count, count > 0 ? totalRating / count : 0.0};
    }
    return result;
}

vector<Rating> Statistics::userRatings(const string& user) const {
    vector<Rating> ratings;
    const User* found = um.findUserByName(user);
    if (!found) {
        return ratings;
    }
    
    const auto& allRatings = rm.getRatings();
    for (const auto& r : allRatings) {
        if (r.getUserName() == found->getUserName()) {
            ratings.push_back(r);
        }
    }
    return ratings;
}

vector<string> Statistics::formatUserLines(const string& user, 
                                           const vector<Rating>& ratings) const {
    vector<string> result;
    
    if (ratings.empty()) {
        result.push_back("  해당 사용자의 평점 이력이 없습니다.");
        return result;
    }
    
    int ratingCount = ratings.size();
    double sumRating = accumulate(ratings.begin(), ratings.end(), 0.0,
        [](double acc, const Rating& r) {
            return acc + r.getUserRating();
        });
    double avg = sumRating / ratingCount;
    
    stringstream ss;
    ss << "  사용자명       : " << user;
    result.push_back(ss.str());
    
    ss.str(""); ss.clear();
    ss << "  평가한 영화 수 : " << ratingCount << "개";
    result.push_back(ss.str());
    
    ss.str(""); ss.clear();
    ss << "  평균 평점     : " << fixed << setprecision(2) << avg;
    result.push_back(ss.str());
    
    ss.str(""); ss.clear();
    ss << "  최고 평점     : " << fixed << setprecision(1) 
       << (*max_element(ratings.begin(), ratings.end(),
           [](const Rating& a, const Rating& b) {
               return a.getUserRating() < b.getUserRating();
           })).getUserRating();
    result.push_back(ss.str());
    
    ss.str(""); ss.clear();
    ss << "  최저 평점     : " << fixed << setprecision(1)
       << (*min_element(ratings.begin(), ratings.end(),
           [](const Rating& a, const Rating& b) {
               return a.getUserRating() < b.getUserRating();
           })).getUserRating();
    result.push_back(ss.str());
    
    return result;
}

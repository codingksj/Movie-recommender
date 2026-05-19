// 협업 필터링 및 장르 유사도를 이용한 추천 기능을 수행하는 클래스

#include "Recommender.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <iomanip>
#include <sstream>

using namespace std;

// 특정 사용자의 평점 목록 반환
vector<Rating> Recommender::getUserRatings(const string& userName) const {
    vector<Rating> result;
    for (const auto& r : ratingManager.getRatings()) {
        if (r.getUserName() == userName) {
            result.push_back(r);
        }
    }
    return result;
}

// 두 사용자 간의 평점 이력 유사도 계산
double Recommender::calculateSimilarity(const vector<Rating>& ratingsA,
                                         const vector<Rating>& ratingsB) const {
    double dotProduct = 0.0;
    double magA = 0.0;
    double magB = 0.0;
    int commonCount = 0;

    for (const auto& rA : ratingsA) {
        for (const auto& rB : ratingsB) {
            if (rA.getMovieTitle() == rB.getMovieTitle()) {
                dotProduct += rA.getUserRating() * rB.getUserRating();
                magA += rA.getUserRating() * rA.getUserRating();
                magB += rB.getUserRating() * rB.getUserRating();
                commonCount++;
            }
        }
    }

    if (commonCount == 0) {
        return 0.0;
    }

    double denominator = sqrt(magA) * sqrt(magB);
    if (denominator == 0.0) {
        return 0.0;
    }

    return dotProduct / denominator;
}

// 특정 사용자와 유사한 사용자 목록 계산
vector<pair<string, double>> Recommender::getSimilarUsers(const string& targetUser, int limit) const {
    vector<Rating> targetRatings = getUserRatings(targetUser);
    if (targetRatings.empty()) {
        return {};
    }

    vector<pair<string, double>> similarities;
    for (const auto& user : userManager.getUsers()) {
        if (user.getUserName() == targetUser) {
            continue;
        }
        vector<Rating> otherRatings = getUserRatings(user.getUserName());
        double sim = calculateSimilarity(targetRatings, otherRatings);
        if (sim > 0.0) {
            similarities.push_back({user.getUserName(), sim});
        }
    }

    sort(similarities.begin(), similarities.end(),
         [](const pair<string, double>& a, const pair<string, double>& b) {
             if (a.second == b.second) return a.first < b.first;
             return a.second > b.second;
         });

    if (limit > 0 && similarities.size() > static_cast<size_t>(limit)) {
        similarities.resize(limit);
    }
    return similarities;
}

// 사용자 기준 협업 필터링 영화 추천 목록 생성
vector<string> Recommender::recommend(const string& targetUser, int K, int N) {
    vector<Rating> targetRatings = getUserRatings(targetUser);
    // 엣지 케이스 1: 대상 사용자의 평점 정보가 전혀 없는 경우 추천 불가로 빈 결과 반환
    if (targetRatings.empty()) {
        return {};
    }

    vector<pair<string, double>> userSims;
    for (const auto& user : userManager.getUsers()) {
        if (user.getUserName() == targetUser) {
            continue;
        }
        vector<Rating> otherRatings = getUserRatings(user.getUserName());
        if (otherRatings.empty()) {
            continue;
        }

        double sim = calculateSimilarity(targetRatings, otherRatings);
        if (sim > 0.0) {
            userSims.push_back({user.getUserName(), sim});
        }
    }

    // 엣지 케이스 2: 유사도가 0보다 큰 이웃 사용자가 아예 존재하지 않는 경우 빈 결과 반환
    if (userSims.empty()) {
        return {};
    }

    sort(userSims.begin(), userSims.end(),
         [](const pair<string, double>& a, const pair<string, double>& b) {
             return a.second > b.second;
         });

    size_t actualK = K;
    if (userSims.size() < actualK) {
        actualK = userSims.size();
    }

    set<string> watchedMovies;
    for (const auto& r : targetRatings) {
        watchedMovies.insert(r.getMovieTitle());
    }

    map<string, double> movieScores;

    for (size_t i = 0; i < actualK; ++i) {
        string neighborName = userSims[i].first;
        double sim = userSims[i].second;
        vector<Rating> neighborRatings = getUserRatings(neighborName);

        for (const auto& r : neighborRatings) {
            if (watchedMovies.find(r.getMovieTitle()) == watchedMovies.end()) {
                movieScores[r.getMovieTitle()] += sim * r.getUserRating();
            }
        }
    }

    vector<pair<string, double>> candidates;
    for (const auto& ms : movieScores) {
        candidates.push_back({ms.first, ms.second});
    }

    // 엣지 케이스 3: 이웃들이 본 영화 중 타겟 사용자가 아직 보지 않은 영화 후보가 없는 경우 빈 결과 반환
    if (candidates.empty()) {
        return {};
    }

    sort(candidates.begin(), candidates.end(),
         [](const pair<string, double>& a, const pair<string, double>& b) {
             if (a.second == b.second) return a.first < b.first;
             return a.second > b.second;
         });

    vector<string> result;
    int count = 0;
    for (const auto& c : candidates) {
        result.push_back(c.first);
        if (++count >= N) {
            break;
        }
    }

    return result;
}

// 두 영화 간의 장르 유사도 계산
double Recommender::calculateGenreSimilarity(const Movie& m1, const Movie& m2) const {
    if (m1.getGenre() == "unknown" || m2.getGenre() == "unknown" ||
        m1.getGenre().empty() || m2.getGenre().empty()) {
        return 0.0;
    }
    string g1 = m1.getGenre();
    string g2 = m2.getGenre();
    transform(g1.begin(), g1.end(), g1.begin(), ::tolower);
    transform(g2.begin(), g2.end(), g2.begin(), ::tolower);

    return (g1 == g2) ? 1.0 : 0.0;
}

// 기준 영화의 장르 기준 추천 영화 목록 생성
vector<string> Recommender::recommendByGenre(const string& targetMovieTitle, int N) const {
    const auto& movies = movieManager.getMovies();
    const Movie* targetMovie = nullptr;
    for (const auto& m : movies) {
        if (m.getTitle() == targetMovieTitle) {
            targetMovie = &m;
            break;
        }
    }

    // 엣지 케이스 1: 추천의 기준이 되는 영화 제목이 DB에 존재하지 않는 경우 빈 결과 반환
    if (!targetMovie) {
        return {};
    }

    vector<pair<Movie, double>> candidates;
    for (const auto& m : movies) {
        // 엣지 케이스 2: 기준이 되는 영화 자체는 추천 결과 목록에서 당연히 제외
        if (m.getTitle() == targetMovieTitle) {
            continue;
        }
        double sim = calculateGenreSimilarity(*targetMovie, m);
        candidates.push_back({m, sim});
    }

    sort(candidates.begin(), candidates.end(), [](const pair<Movie, double>& a, const pair<Movie, double>& b) {
        if (a.second != b.second) {
            return a.second > b.second;
        }
        if (a.first.getAverageRating() != b.first.getAverageRating()) {
            return a.first.getAverageRating() > b.first.getAverageRating();
        }
        return a.first.getTitle() < b.first.getTitle();
    });

    vector<string> result;
    int count = 0;
    for (const auto& pair : candidates) {
        if (pair.second > 0.0) {
            stringstream ss;
            ss << pair.first.getTitle() << " (장르: " << pair.first.getGenre() 
               << ", 평점: " << fixed << setprecision(1) << pair.first.getAverageRating() << ")";
            result.push_back(ss.str());
            if (++count >= N) {
                break;
            }
        }
    }
    return result;
}

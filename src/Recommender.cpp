// 사용자에게 맞춤형 영화 리스트를 제공하여 만족도를 높이기 위해 설계된 추천 엔진 클래스

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
#include <iterator>

using namespace std;

// 특정 사용자의 선호도를 파악하기 위해 시스템에 기록된 모든 평점 중 해당 사용자의 것만 필터링하여 반환함
vector<Rating> Recommender::getUserRatings(const string& userName) const {
    vector<Rating> result;
    for (const auto& r : ratingManager.getRatings()) {
        if (r.getUserName() == userName) {
            result.push_back(r);
        }
    }
    return result;
}

// 코사인 유사도를 이용하여 두 사용자의 취향이 얼마나 비슷한지 정량화하기 위함
double Recommender::calculateSimilarity(const vector<Rating>& ratingsA,
                                         const vector<Rating>& ratingsB) const {
    double dotProduct = 0.0;
    double magA = 0.0, magB = 0.0;
    int commonCount = 0;

    // 전체 평가한 이력을 기준으로 사용자 벡터의 크기(Norm) 계산
    for (const auto& rA : ratingsA) {
        magA += rA.getUserRating() * rA.getUserRating();
    }
    for (const auto& rB : ratingsB) {
        magB += rB.getUserRating() * rB.getUserRating();
    }

    // 공통으로 평가한 영화 매칭 (연산자 오버로딩 활용)
    for (const auto& rA : ratingsA) {
        for (const auto& rB : ratingsB) {
            if (rA == rB) {
                dotProduct += rA.getUserRating() * rB.getUserRating();
                commonCount++;
            }
        }
    }

    if (commonCount == 0 || magA == 0.0 || magB == 0.0) {
        return 0.0;
    }

    return dotProduct / (sqrt(magA) * sqrt(magB));
}

// 대상 사용자와 취향이 비슷한 이웃 사용자들을 찾아내어 협업 필터링의 기초 데이터를 구성하기 위함
vector<pair<string, double>> Recommender::getSimilarUsers(const string& targetUser, int limit) const {
    vector<Rating> targetRatings = getUserRatings(targetUser);
    if (targetRatings.empty()) { return {}; }

    vector<pair<string, double>> similarities;
    for (const auto& user : userManager.getUsers()) {
        if (user.getUserName() == targetUser) { continue; }

        vector<Rating> otherRatings = getUserRatings(user.getUserName());
        double sim = calculateSimilarity(targetRatings, otherRatings);
        if (sim > 0.0) {
            similarities.push_back({user.getUserName(), sim});
        }
    }

    sort(similarities.begin(), similarities.end(),
         [](const pair<string, double>& a, const pair<string, double>& b) {
             if (a.second == b.second) { return a.first < b.first; }
             return a.second > b.second;
         });

    if (limit > 0 && (int)similarities.size() > limit) {
        similarities.resize(limit);
    }
    return similarities;
}

// 이웃 사용자들의 평점 데이터를 바탕으로 대상 사용자가 보지 않은 영화의 예상 점수를 계산하기 위함 (헬퍼 함수)
map<string, double> Recommender::predictMovieScores(const vector<pair<string, double>>& userSims, 
                                                    const set<string>& watchedMovies, size_t actualK) {
    map<string, double> movieScores;
    map<string, double> sumSims; // 영화별 유사도 합계 저장을 위함

    for (size_t i = 0; i < actualK; ++i) {
        string neighborName = userSims[i].first;
        double sim = userSims[i].second;
        vector<Rating> neighborRatings = getUserRatings(neighborName);

        for (const auto& r : neighborRatings) {
            if (watchedMovies.find(r.getMovieTitle()) == watchedMovies.end()) {
                movieScores[r.getMovieTitle()] += sim * r.getUserRating();
                sumSims[r.getMovieTitle()] += sim;
            }
        }
    }

    // 유사도 가중치 합으로 나누어 정규화 (가중 평균 계산)
    for (auto& pair : movieScores) {
        if (sumSims[pair.first] > 0.0) {
            pair.second /= sumSims[pair.first];
        }
    }

    return movieScores;
}

// 대상 사용자와 유사한 취향을 가진 K명의 이웃 데이터를 종합하여 가장 높은 예상 점수를 받은 N개의 영화를 추천하기 위함
vector<string> Recommender::recommend(const string& targetUser, int K, int N, const string& genre) {
    vector<Rating> targetRatings = getUserRatings(targetUser);
    if (targetRatings.empty()) { return {}; }

    vector<pair<string, double>> userSims = getSimilarUsers(targetUser, 0);
    if (userSims.empty()) { return {}; }

    size_t actualK = min((size_t)K, userSims.size());

    set<string> watchedMovies;
    for (const auto& r : targetRatings) { watchedMovies.insert(r.getMovieTitle()); }

    map<string, double> movieScores = predictMovieScores(userSims, watchedMovies, actualK);

    vector<pair<string, double>> candidates;
    vector<Movie> genreFilteredMovies;
    if (!genre.empty()) {
        genreFilteredMovies = filterByGenre(genre);
    }

    for (const auto& pair : movieScores) {
        if (!genre.empty()) {
            bool genreMatch = false;
            for (const auto& m : genreFilteredMovies) {
                if (m.getTitle() == pair.first) {
                    genreMatch = true;
                    break;
                }
            }
            if (!genreMatch) {
                continue;
            }
        }
        candidates.push_back(pair);
    }

    if (candidates.empty()) { return {}; }

    sort(candidates.begin(), candidates.end(),
         [](const pair<string, double>& a, const pair<string, double>& b) {
             if (a.second == b.second) { return a.first < b.first; }
             return a.second > b.second;
         });

    vector<string> result;
    for (int i = 0; i < min(N, (int)candidates.size()); ++i) {
        result.push_back(candidates[i].first);
    }
    return result;
}

// 두 영화 간의 메타데이터(장르)가 얼마나 일치하는지 판별하여 내용 기반 추천의 기준을 마련하기 위함
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

// 장르 문자열(대소문자 무시)이 정확히 일치하는 영화들만 반환하는 헬퍼
vector<Movie> Recommender::filterByGenre(const string& genre) const {
    vector<Movie> result;
    if (genre.empty()) { return result; }
    string g = genre;
    transform(g.begin(), g.end(), g.begin(), ::tolower);

    const auto& movies = movieManager.getMovies();
    copy_if(movies.begin(), movies.end(), back_inserter(result),
        [&g](const Movie& m) {
            string mg = m.getGenre();
            transform(mg.begin(), mg.end(), mg.begin(), ::tolower);
            return mg == g;
        });
    return result;
}

// 사용자가 관심을 가지는 특정 영화와 유사한 분위기/내용(장르)을 가진 영화 목록을 제공하기 위함
vector<string> Recommender::recommendByGenre(const string& targetMovieTitle, int N) const {
    const auto& allMovies = movieManager.getMovies();
    const Movie* targetMovie = nullptr;
    for (const auto& m : allMovies) {
        if (m == targetMovieTitle) {
            targetMovie = &m;
            break;
        }
    }

    if (!targetMovie) { return {}; }

    // Use filterByGenre to narrow candidates to the same genre (faster and clearer)
    vector<Movie> sameGenre = filterByGenre(targetMovie->getGenre());
    vector<pair<Movie, double>> candidates;
    for (const auto& m : sameGenre) {
        if (m == targetMovieTitle) { continue; }
        double sim = calculateGenreSimilarity(*targetMovie, m);
        candidates.push_back({m, sim});
    }

    sort(candidates.begin(), candidates.end(), [](const pair<Movie, double>& a, const pair<Movie, double>& b) {
        if (a.second != b.second) { return a.second > b.second; }
        if (a.first.getAverageRating() != b.first.getAverageRating()) { return a.first.getAverageRating() > b.first.getAverageRating(); }
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
            if (++count >= N) { break; }
        }
    }
    return result;
}

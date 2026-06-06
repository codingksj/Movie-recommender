// 협업 필터링·장르 기반 추천 엔진

#include "Recommender.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <unordered_map>
#include <iomanip>
#include <sstream>
#include <iterator>

using namespace std;

vector<Rating> Recommender::getUserRatings(const string& user) const {
    vector<Rating> result;
    string key = Base::normalizeString(user);
    for (const auto& r : rm.getRatings()) {
        if (Base::normalizeString(r.getUserName()) == key) {
            result.push_back(r);
        }
    }
    return result;
}

// 두 사용자의 평점 벡터 간 코사인 유사도를 계산하여
// 사용자 간 영화 취향 유사성을 측정
double Recommender::calculateSimilarity(const vector<Rating>& a, const vector<Rating>& b) const {
    unordered_map<string, double> mapA;
    mapA.reserve(a.size());
    double magA = 0.0;
    for (const auto& r : a) {
        mapA[r.getMovieTitle()] = r.getUserRating();
        magA += r.getUserRating() * r.getUserRating();
    }

    unordered_map<string, double> mapB;
    mapB.reserve(b.size());
    double magB = 0.0;
    for (const auto& r : b) {
        mapB[r.getMovieTitle()] = r.getUserRating();
        magB += r.getUserRating() * r.getUserRating();
    }

    double dot = 0.0;
    for (const auto& [title, scoreA] : mapA) {
        auto it = mapB.find(title);
        if (it != mapB.end()) {
            dot += scoreA * it->second;
        }
    }

    if (dot == 0.0 || magA == 0.0 || magB == 0.0) {
        return 0.0;
    }

    return dot / (sqrt(magA) * sqrt(magB));
}

vector<pair<string, double>> Recommender::getSimilarUsers(const string& user, int limit) const {
    vector<Rating> base = getUserRatings(user);
    if (base.empty()) { return {}; }

    vector<pair<string, double>> similarities;
    for (const auto& u : um.getUsers()) {
        if (u.getUserName() == user) { continue; }

        vector<Rating> other = getUserRatings(u.getUserName());
        double sim = calculateSimilarity(base, other);
        if (sim > 0.0) {
            similarities.push_back({u.getUserName(), sim});
        }
    }

    sort(similarities.begin(), similarities.end(),
         [](const pair<string, double>& x, const pair<string, double>& y) {
             if (x.second == y.second) { return x.first < y.first; }
             return x.second > y.second;
         });

    if (limit > 0 && (int)similarities.size() > limit) {
        similarities.resize(limit);
    }
    return similarities;
}

// 유사 사용자들의 평점을 가중 평균으로 계산하여
// 타겟 사용자의 미시청 영화에 대한 예상 점수 도출 (협업 필터링 추천)
// @param k 고려할 유사 사용자 수 @param watched 미시청 영화 유무
map<string, double> Recommender::predictMovieScores(const vector<pair<string, double>>& userSims,
                                                    const set<string>& watched, size_t k) const {
    map<string, double> movieScores;
    map<string, double> sumSims;

    for (size_t i = 0; i < k; ++i) {
        string neighbor = userSims[i].first;
        double sim = userSims[i].second;
        vector<Rating> neighborRatings = getUserRatings(neighbor);

        for (const auto& r : neighborRatings) {
            if (watched.find(r.getMovieTitle()) == watched.end()) {
                movieScores[r.getMovieTitle()] += sim * r.getUserRating();
                sumSims[r.getMovieTitle()] += sim;
            }
        }
    }

    for (auto& entry : movieScores) {
        if (sumSims[entry.first] > 0.0) {
            entry.second /= sumSims[entry.first];
        }
    }

    return movieScores;
}

vector<string> Recommender::recommend(const string& user, int k, int n, const string& genre) {
    vector<Rating> base = getUserRatings(user);
    if (base.empty()) { return {}; }

    vector<pair<string, double>> userSims = getSimilarUsers(user, 0);
    if (userSims.empty()) { return {}; }

    size_t neighbors = min((size_t)k, userSims.size());

    set<string> watched;
    for (const auto& r : base) { watched.insert(r.getMovieTitle()); }

    map<string, double> movieScores = predictMovieScores(userSims, watched, neighbors);

    vector<pair<string, double>> candidates;
    vector<Movie> genreFiltered;
    if (!genre.empty()) {
        genreFiltered = filterByGenre(genre);
    }

    for (const auto& entry : movieScores) {
        if (!genre.empty()) {
            bool genreMatch = false;
            for (const auto& m : genreFiltered) {
                if (m.getTitle() == entry.first) {
                    genreMatch = true;
                    break;
                }
            }
            if (!genreMatch) {
                continue;
            }
        }
        candidates.push_back(entry);
    }

    if (candidates.empty()) { return {}; }

    sort(candidates.begin(), candidates.end(),
         [](const pair<string, double>& x, const pair<string, double>& y) {
             if (x.second == y.second) { return x.first < y.first; }
             return x.second > y.second;
         });

    vector<string> result;
    for (int i = 0; i < min(n, (int)candidates.size()); ++i) {
        result.push_back(candidates[i].first);
    }
    return result;
}

double Recommender::calculateGenreSimilarity(const Movie& m1, const Movie& m2) const {
    if (m1.getGenre() == "unknown" || m2.getGenre() == "unknown" ||
        m1.getGenre().empty() || m2.getGenre().empty()) {
        return 0.0;
    }
    string g1 = Base::normalizeString(m1.getGenre());
    string g2 = Base::normalizeString(m2.getGenre());

    return (g1 == g2) ? 1.0 : 0.0;
}

vector<Movie> Recommender::filterByGenre(const string& genre) const {
    vector<Movie> result;
    if (genre.empty()) { return result; }
    string g = Base::normalizeString(genre);

    const auto& movies = mm.getMovies();
    copy_if(movies.begin(), movies.end(), back_inserter(result),
        [&g](const Movie& m) {
            return Base::normalizeString(m.getGenre()) == g;
        });
    return result;
}

vector<string> Recommender::recommendByGenre(const string& title, int n) const {
    const Movie* base = mm.findMovieByTitle(title);
    if (!base) { return {}; }

    vector<Movie> sameGenre = filterByGenre(base->getGenre());
    vector<pair<Movie, double>> candidates;
    string baseKey = Base::normalizeString(base->getTitle());
    for (const auto& m : sameGenre) {
        if (Base::normalizeString(m.getTitle()) == baseKey) { continue; }
        double sim = calculateGenreSimilarity(*base, m);
        candidates.push_back({m, sim});
    }

    sort(candidates.begin(), candidates.end(), [](const pair<Movie, double>& x, const pair<Movie, double>& y) {
        if (x.second != y.second) { return x.second > y.second; }
        if (x.first.getAverageRating() != y.first.getAverageRating()) { return x.first.getAverageRating() > y.first.getAverageRating(); }
        return x.first.getTitle() < y.first.getTitle();
    });

    vector<string> result;
    int count = 0;
    for (const auto& entry : candidates) {
        if (entry.second > 0.0) {
            stringstream ss;
            ss << entry.first.getTitle() << " (장르: " << entry.first.getGenre()
               << ", 평점: " << fixed << setprecision(1) << entry.first.getAverageRating() << ")";
            result.push_back(ss.str());
            if (++count >= n) { break; }
        }
    }
    return result;
}

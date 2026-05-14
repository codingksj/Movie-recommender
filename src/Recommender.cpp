#include "Recommender.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <iomanip>
#include <utility>

using namespace std;

vector<Rating> Recommender::getUserRatings(const string& userName) const {
    vector<Rating> result;
    for (const auto& r : ratingManager.getRatings()) {
        if (r.getUserName() == userName) {
            result.push_back(r);
        }
    }
    return result;
}


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
    if (denominator == 0.0) return 0.0;

    return dotProduct / denominator;
}

void Recommender::findSimilarUsers() {
    string targetUser;
    cout << "유사 사용자를 찾을 사용자 이름 입력: ";
    cin >> targetUser;

    if (userManager.findUserByName(targetUser) == nullptr) {
        cout << "존재하지 않는 사용자입니다.\n";
        return;
    }

    vector<Rating> targetRatings = getUserRatings(targetUser);

    if (targetRatings.empty()) {
        cout << "해당 사용자의 평점 이력이 없어 유사도를 계산할 수 없습니다.\n";
        return;
    }

    vector<pair<string, double>> similarities;
    for (const auto& user : userManager.getUsers()) {
        if (user.getUserName() == targetUser) continue;
        vector<Rating> otherRatings = getUserRatings(user.getUserName());
        double sim = calculateSimilarity(targetRatings, otherRatings);
        similarities.push_back({user.getUserName(), sim});
    }

    sort(similarities.begin(), similarities.end(),
         [](const pair<string, double>& a, const pair<string, double>& b) {
             if (a.second == b.second) return a.first < b.first;
             return a.second > b.second;
         });

    cout << "\n[ " << targetUser << "와 유사한 사용자 상위 3명 ]\n";
    int count = 0;
    for (const auto& s : similarities) {
        if (s.second <= 0.0) continue;
        cout << ++count << ". " << s.first
             << " (유사도: " << fixed << setprecision(3) << s.second << ")\n";
        if (count >= 3) break;
    }
    if (count == 0) cout << "유사한 사용자가 없습니다.\n";
}

vector<string> Recommender::recommend(const string& targetUser, int K, int N) {
    vector<Rating> targetRatings = getUserRatings(targetUser);
    if (targetRatings.empty()) {
        return {};
    }

    vector<pair<string, double>> userSims;
    for (const auto& user : userManager.getUsers()) {
        if (user.getUserName() == targetUser) continue;
        vector<Rating> otherRatings = getUserRatings(user.getUserName());
        if (otherRatings.empty()) continue;

        double sim = calculateSimilarity(targetRatings, otherRatings);
        if (sim > 0.0) {
            userSims.push_back({user.getUserName(), sim});
        }
    }

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
        if (++count >= N) break;
    }

    return result;
}

void Recommender::recommendMenu() {
    string targetUser;
    cout << "추천을 받을 사용자 이름 입력: ";
    cin >> targetUser;

    if (userManager.findUserByName(targetUser) == nullptr) {
        cout << "존재하지 않는 사용자입니다.\n";
        return;
    }

    vector<string> recommended = recommend(targetUser, 3, 5);

    if (recommended.empty()) {
        cout << "추천할 만한 새로운 영화가 없습니다.\n";
        return;
    }

    cout << "\n[ " << targetUser << "님을 위한 추천 영화 ]\n";
    for (size_t i = 0; i < recommended.size(); ++i) {
        cout << (i + 1) << ". " << recommended[i] << "\n";
    }
}

#include "recommend.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>
#include <iomanip>

using namespace std;

double Recommend::calculateSimilarity(const string& userName1, const string& userName2) const {
    const auto& allRatings = ratingManager.getRatings();
    
    vector<Rating> user1Ratings;
    vector<Rating> user2Ratings;

    for (const auto& r : allRatings) {
        if (r.getUserName() == userName1) user1Ratings.push_back(r);
        if (r.getUserName() == userName2) user2Ratings.push_back(r);
    }

    int commonCount = 0;
    double scoreDiffSum = 0;

    for (const auto& r1 : user1Ratings) {
        for (const auto& r2 : user2Ratings) {
            if (r1.getMovieTitle() == r2.getMovieTitle()) {
                commonCount++;
                scoreDiffSum += abs(r1.getUserRating() - r2.getUserRating());
            }
        }
    }

    if (commonCount == 0) {
        return -999.0; // -INF 대용
    }

    return (double)(commonCount * 10 - scoreDiffSum);
}

void Recommend::findSimilarUsers() {
    string targetUser;
    cout << "유사 사용자를 찾을 사용자 이름 입력: ";
    cin >> targetUser;

    if (userManager.findUserByName(targetUser) == nullptr) {
        cout << "존재하지 않는 사용자입니다.\n";
        return;
    }

    vector<pair<string, double>> similarities;
    for (const auto& user : userManager.getUsers()) {
        if (user.getUserName() == targetUser) continue;
        double sim = calculateSimilarity(targetUser, user.getUserName());
        similarities.push_back({user.getUserName(), sim});
    }

    sort(similarities.begin(), similarities.end(), [](const auto& a, const auto& b) {
        if (a.second == b.second) return a.first < b.first; // 유사도 같으면 이름순 (랜덤 대용)
        return a.second > b.second;
    });

    cout << "\n[ " << targetUser << "와 유사한 사용자 상위 3명 ]\n";
    int count = 0;
    for (const auto& s : similarities) {
        if (s.second <= -999.0) continue;
        cout << ++count << ". " << s.first << " (유사도: " << fixed << setprecision(1) << s.second << ")\n";
        if (count >= 3) break;
    }
    if (count == 0) cout << "유사한 사용자가 없습니다.\n";
}

void Recommend::recommend() {
    string targetUser;
    cout << "추천을 받을 사용자 이름 입력: ";
    cin >> targetUser;

    if (userManager.findUserByName(targetUser) == nullptr) {
        cout << "존재하지 않는 사용자입니다.\n";
        return;
    }

    // 1. 유사도 계산
    vector<pair<string, double>> userSims;
    for (const auto& user : userManager.getUsers()) {
        if (user.getUserName() == targetUser) continue;
        double sim = calculateSimilarity(targetUser, user.getUserName());
        if (sim > -999.0) {
            userSims.push_back({user.getUserName(), sim});
        }
    }

    if (userSims.empty()) {
        cout << "유사한 사용자가 없어 추천할 수 없습니다.\n";
        return;
    }

    // 유사도 순 정렬
    sort(userSims.begin(), userSims.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    // 상위 K명 (K=3)
    size_t K = 3;
    if (userSims.size() < K) K = userSims.size();

    // 2. 타겟 사용자가 본 영화 목록
    vector<string> watchedMovies;
    for (const auto& r : ratingManager.getRatings()) {
        if (r.getUserName() == targetUser) {
            watchedMovies.push_back(r.getMovieTitle());
        }
    }

    // 3. 예상 평점 계산
    map<string, pair<double, double>> movieScores; // movieTitle -> {sum(sim * rating), sum(abs(sim))}

    for (int i = 0; i < K; ++i) {
        string otherUser = userSims[i].first;
        double sim = userSims[i].second;
        if (sim <= 0) continue; // 유사도가 양수인 경우만 고려 (가중 평균을 위해)

        for (const auto& r : ratingManager.getRatings()) {
            if (r.getUserName() == otherUser) {
                // 이미 본 영화는 제외
                if (find(watchedMovies.begin(), watchedMovies.end(), r.getMovieTitle()) == watchedMovies.end()) {
                    movieScores[r.getMovieTitle()].first += sim * r.getUserRating();
                    movieScores[r.getMovieTitle()].second += abs(sim);
                }
            }
        }
    }

    vector<pair<string, double>> predictions;
    for (const auto& ms : movieScores) {
        if (ms.second.second > 0) {
            predictions.push_back({ms.first, ms.second.first / ms.second.second});
        }
    }

    if (predictions.empty()) {
        cout << "추천할 만한 새로운 영화가 없습니다.\n";
        return;
    }

    // 예상 평점 높은 순 정렬
    sort(predictions.begin(), predictions.end(), [](const auto& a, const auto& b) {
        if (a.second == b.second) return a.first < b.first;
        return a.second > b.second;
    });

    cout << "\n[ " << targetUser << "님을 위한 추천 영화 ]\n";
    int N = 5;
    int count = 0;
    for (const auto& p : predictions) {
        cout << ++count << ". " << p.first << " (예상 평점: " << fixed << setprecision(1) << p.second << ")\n";
        if (count >= N) break;
    }
}

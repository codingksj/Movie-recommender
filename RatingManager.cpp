#include "RatingManager.h"
#include <iostream>
using std::cout;
using std::cin;
using std::string;
using std::vector;
void RatingManager::addRating(MovieManager& movieManager, UserManager& userManager) {
    string userName, movieTitle;
    double score;
    cout << "사용자 이름: ";
    cin.ignore();
    if (!std::getline(cin, userName) || userName.empty()) return;
    if (!userManager.findUserByName(userName)) {
        cout << "등록되지 않은 사용자입니다.\n";
        return;
    }
    cout << "영화 제목: ";
    if (!std::getline(cin, movieTitle) || movieTitle.empty()) return;
    Movie* movie = movieManager.findMovieByTitle(movieTitle);
    if (!movie) {
        cout << "해당 제목의 영화를 찾을 수 없습니다.\n";
        return;
    }
    cout << "평점 (0.0 ~ 5.0): ";
    if (!(cin >> score)) {
        cin.clear(); cin.ignore(1000, '\n');
        return;
    }
    if (!movie->addRating(score)) return;
    ratings.push_back(Rating(userName, movieTitle, score));
    cout << "평점이 등록되었습니다.\n";
}
void RatingManager::printRatingsByMovie() {
    string movieTitle;
    cout << "평점을 볼 영화 제목: ";
    cin.ignore();
    std::getline(cin, movieTitle);
    bool found = false;
    cout << "\n#### [" << movieTitle << "] 평점 목록 ####\n";
    for (const auto& r : ratings) {
        if (r.getMovieTitle() == movieTitle) {
            cout << r << '\n';
            found = true;
        }
    }
    if (!found) cout << "등록된 평점이 없습니다.\n";
}

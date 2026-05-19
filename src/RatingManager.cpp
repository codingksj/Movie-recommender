// 평점 데이터를 추가 및 관리하는 클래스

#include "RatingManager.h"
#include "menu.h"
#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::cin;
using std::string;
using std::vector;

// 평점 점수 범위 상수
const double MIN_RATE = 0.0;
const double MAX_RATE = 10.0;
const int IGN_COUNT = 1000;

// 신규 평점 입력 프롬프트 처리
void RatingManager::addRating(MovieManager& movieManager, UserManager& userManager) {
    string userName, movieTitle;
    double score;

    cout << "사용자 이름: ";
    cin.ignore();
    if (!std::getline(cin, userName) || userName.empty()) {
        return;
    }

    if (!userManager.findUserByName(userName)) {
        cout << "[" << userName << "]은(는) 등록되지 않은 사용자입니다.\n";
        return;
    }

    cout << "영화 제목: ";
    if (!std::getline(cin, movieTitle) || movieTitle.empty()) {
        return;
    }

    Movie* movie = movieManager.findMovieByTitle(movieTitle);
    if (!movie) {
        cout << "[" << movieTitle << "] 제목의 영화를 찾을 수 없습니다.\n";
        return;
    }

    cout << "평점 (" << MIN_RATE << " ~ " << MAX_RATE << "): ";
    if (!(cin >> score)) {
        cin.clear();
        cin.ignore(IGN_COUNT, '\n');
        return;
    }

    if (!movie->addRating(score)) {
        return;
    }

    ratings.push_back(Rating(userName, movieTitle, score));
    cout << "평점이 등록되었습니다. [사용자: " << userName << ", 영화: " << movieTitle << ", 평점: " << score << "]\n";
}

// 특정 영화에 대한 평점 목록 출력
void RatingManager::printRatingsByMovie() {
    string movieTitle;
    cout << "평점을 볼 영화 제목: ";
    cin.ignore();
    std::getline(cin, movieTitle);

    vector<string> lines;
    for (const auto& r : ratings) {
        if (r.getMovieTitle() == movieTitle) {
            std::stringstream ss;
            ss << " " << r;
            lines.push_back(ss.str());
        }
    }

    if (lines.empty()) {
        cout << "등록된 평점이 없습니다.\n";
        return;
    }

    Menu::showDynamicResult("[" + movieTitle + "] 평점 목록", lines);
}

// 파일에서 평점 데이터 불러오기
void RatingManager::loadFromFile() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        cout << "파일을 열 수 없습니다: " << filePath << "\n";
        return;
    }

    string line;
    std::getline(file, line);
    
    int count = 0;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        string name, title, ratingStr;

        std::getline(ss, name, ',');
        std::getline(ss, title, ',');
        std::getline(ss, ratingStr, ',');

        if (!name.empty()) {
            ratings.push_back(Rating(name, title, std::stod(ratingStr)));
            count++;
        }
    }
    file.close();
    cout << "평점 데이터 로드 완료 (" << count << "건)\n";
}

// 파일로 평점 데이터 저장하기
void RatingManager::saveToFile() {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        cout << "파일을 저장할 수 없습니다: " << filePath << "\n";
        return;
    }

    file << "userName,movieTitle,userRating\n";
    for (const auto& r : ratings) {
        file << r.getUserName() << "," << r.getMovieTitle() << "," << r.getUserRating() << "\n";
    }
    file.close();
    cout << "평점 데이터 저장 완료\n";
}

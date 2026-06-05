// 시스템 내 영화에 대한 사용자 평점 데이터를 관리하기 위해 설계된 클래스

#include "RatingManager.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>

using namespace std;
using Clock = chrono::high_resolution_clock;
using Ms = chrono::microseconds;

// 신규 평점을 시스템에 등록하기 위해 제약조건과 데이터 유효성을 검증하고 저장함
pair<RatingResult, double> RatingManager::addRating(const string& userName, const string& movieTitle, double score, MovieManager& movieManager, UserManager& userManager) {
    auto start = Clock::now();

    const User* user = userManager.findUserByName(userName);
    if (!user) {
        auto end = Clock::now();
        return {RatingResult::USER_NOT_FOUND, chrono::duration_cast<Ms>(end - start).count() / 1000.0};
    }

    Movie* movie = movieManager.findMovieByTitle(movieTitle);
    if (!movie) {
        auto end = Clock::now();
        return {RatingResult::MOVIE_NOT_FOUND, chrono::duration_cast<Ms>(end - start).count() / 1000.0};
    }

    if (score < MovieConstants::MIN_RATE || score > MovieConstants::MAX_RATE) {
        auto end = Clock::now();
        return {RatingResult::INVALID_SCORE, chrono::duration_cast<Ms>(end - start).count() / 1000.0};
    }

    // 중복 평가 체크
    for (const auto& r : ratings) {
        if (r.getUserName() == userName && r.getMovieTitle() == movie->getTitle()) {
            auto end = Clock::now();
            return {RatingResult::DUPLICATE_RATING, chrono::duration_cast<Ms>(end - start).count() / 1000.0};
        }
    }

    if (!movie->addRating(score)) {
        auto end = Clock::now();
        return {RatingResult::INVALID_SCORE, chrono::duration_cast<Ms>(end - start).count() / 1000.0};
    }

    // 사용자 이름과 영화 제목은 내부 저장된 정규값을 사용
    ratings.push_back(Rating(user->getUserName(), movie->getTitle(), score));
    
    auto end = Clock::now();
    double ms = chrono::duration_cast<Ms>(end - start).count() / 1000.0;

    return {RatingResult::SUCCESS, ms};
}

// 특정 영화에 등록된 모든 평점을 포맷팅된 문자열 목록으로 반환하여 외부에 제공하기 위함
vector<string> RatingManager::getRatingsByMovieFormatted(const string& movieTitle) const {
    vector<string> lines;
    for (const auto& r : ratings) {
        if (r == movieTitle) {
            stringstream ss;
            ss << " " << r;
            lines.push_back(ss.str());
        }
    }
    return lines;
}

// 파일에서 평점 데이터를 메모리로 불러오기 위함
double RatingManager::loadFromFile() {
    ratings.clear();
    auto start = Clock::now();
    try {
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "파일을 열 수 없습니다: " << filePath << "\n";
            return 0.0;
        }
        string line;
        getline(file, line);
        int count = 0;
        while (getline(file, line)) {
            if (line.empty()) { continue; }
            stringstream ss(line);
            string name, title, ratingStr;
            getline(ss, name, ',');
            getline(ss, title, ',');
            getline(ss, ratingStr, ',');
            if (!name.empty()) {
                try {
                    ratings.push_back(Rating(name, title, stod(ratingStr)));
                    count++;
                } catch (const exception &e) {
                    // skip malformed
                }
            }
        }
        file.close();
    } catch (const exception &e) {
        cerr << "RatingManager::loadFromFile 예외 발생: " << e.what() << "\n";
    }
    auto end = Clock::now();
    return chrono::duration_cast<Ms>(end - start).count() / 1000.0;
}

// 메모리의 평점 데이터를 파일에 안전하게 보관하기 위함
double RatingManager::saveToFile() {
    auto start = Clock::now();
    try {
        ofstream file(filePath);
        if (!file.is_open()) {
            cerr << "파일을 저장할 수 없습니다: " << filePath << "\n";
            return 0.0;
        }

        file << "userName,movieTitle,userRating\n";
        for (const auto& r : ratings) {
            file << r.getUserName() << "," << r.getMovieTitle() << "," << r.getUserRating() << "\n";
        }
        file.close();
    } catch (const exception& e) {
        cerr << "RatingManager::saveToFile 예외 발생: " << e.what() << "\n";
    }
    auto end = Clock::now();
    return chrono::duration_cast<Ms>(end - start).count() / 1000.0;
}

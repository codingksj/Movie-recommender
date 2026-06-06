// 평점 CRUD 및 CSV 영속화

#include "RatingManager.h"
#include "MovieConstant.h"

using namespace MC::Score;
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <set>

using namespace std;
using Clock = chrono::high_resolution_clock;
using Ms = chrono::microseconds;

pair<RatingResult, double> RatingManager::addRating(const string& user, const string& title, double score,
                                                    MovieManager& mm, UserManager& um) {
    auto start = Clock::now();

    const User* found = um.findUserByName(user);
    if (!found) {
        auto end = Clock::now();
        return {RatingResult::USER_NOT_FOUND, chrono::duration_cast<Ms>(end - start).count() / 1000.0};
    }

    Movie* movie = mm.findMovieByTitle(title);
    if (!movie) {
        auto end = Clock::now();
        return {RatingResult::MOVIE_NOT_FOUND, chrono::duration_cast<Ms>(end - start).count() / 1000.0};
    }

    if (score < MIN || score > MAX) {
        auto end = Clock::now();
        return {RatingResult::INVALID_SCORE, chrono::duration_cast<Ms>(end - start).count() / 1000.0};
    }

    for (const auto& r : ratings) {
        if (r.getUserName() == user && r.getMovieTitle() == movie->getTitle()) {
            auto end = Clock::now();
            return {RatingResult::DUPLICATE_RATING, chrono::duration_cast<Ms>(end - start).count() / 1000.0};
        }
    }

    if (!movie->addRating(score)) {
        auto end = Clock::now();
        return {RatingResult::INVALID_SCORE, chrono::duration_cast<Ms>(end - start).count() / 1000.0};
    }

    ratings.push_back(Rating(found->getUserName(), movie->getTitle(), score));

    auto end = Clock::now();
    double ms = chrono::duration_cast<Ms>(end - start).count() / 1000.0;

    return {RatingResult::SUCCESS, ms};
}

vector<string> RatingManager::getRatingsByMovieFormatted(const string& title) const {
    vector<string> lines;
    for (const auto& r : ratings) {
        if (r == title) {
            stringstream ss;
            ss << " " << r;
            lines.push_back(ss.str());
        }
    }
    return lines;
}

void RatingManager::mergeRatingsToMovies(MovieManager& mm) const {
    std::set<string> resetTitles;
    for (const auto& r : ratings) {
        Movie* movie = mm.findMovieByTitle(r.getMovieTitle());
        if (!movie) {
            continue;
        }
        if (resetTitles.insert(movie->getTitle()).second) {
            movie->resetRatingAggregate();
        }
        movie->mergeRating(r.getUserRating());
    }
}

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
                    // 형식이 잘못된 평점 데이터는 스킵 (CSV 파싱 오류 방지)
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

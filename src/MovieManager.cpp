// 시스템 내 영화 데이터를 추가, 검색 및 관리하기 위해 설계된 클래스

#include "MovieManager.h"
#include <iostream>
#include <exception>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iterator>

using namespace std;
using Clock = chrono::high_resolution_clock;
using Ms = chrono::microseconds;

// 신규 영화를 시스템에 등록하기 위해 중복 검사 후 데이터를 추가함
pair<MovieResult, double> MovieManager::addMovie(const string& title, int year, const string& genre) {
    auto start = Clock::now();
    try {
        Movie target(title, year, genre);
        for (const auto& m : movies) {
            if (m == target) {
                auto end = Clock::now();
                return {MovieResult::DUPLICATE_MOVIE, chrono::duration_cast<Ms>(end - start).count() / 1000.0};
            }
        }
        movies.push_back(Movie(title, year, genre));
    } catch (const exception &e) {
        cerr << "addMovie 예외 발생: " << e.what() << "\n";
    }
    auto end = Clock::now();
    return {MovieResult::SUCCESS, chrono::duration_cast<Ms>(end - start).count() / 1000.0};
}

// 주어진 쿼리로 영화 제목을 검색하고 포맷팅된 문자열 목록을 반환함
pair<vector<string>, double> MovieManager::searchByTitleFormatted(const string& query, string& outQuery) const {
    string queryLower = query;
    for (char &c : queryLower) { c = tolower(c); }
    
    auto searchStart = Clock::now();
    vector<pair<Movie, string>> exactMatches;
    vector<pair<Movie, string>> partialMatches;
    size_t maxTitleLen = 0, maxGenreLen = 0;
    
    for (const auto &m : movies) {
        string titleLower = m.getTitle();
        for (char &c : titleLower) { c = tolower(c); }
        
        size_t pos = titleLower.find(queryLower);
        if (pos != string::npos) {
            string originalTitle = m.getTitle();
            string leftPart = originalTitle.substr(0, pos);
            string mid = originalTitle.substr(pos, query.length());
            string right = originalTitle.substr(pos + query.length());
            string highlightedTitle = leftPart + "[" + mid + "]" + right;
            auto entry = make_pair(m, highlightedTitle);

            if (Base::normalizeString(m.getTitle()) == Base::normalizeString(query)) {
                exactMatches.push_back(entry);
            } else {
                partialMatches.push_back(entry);
            }
            
            if (highlightedTitle.length() > maxTitleLen) { maxTitleLen = highlightedTitle.length(); }
            if (m.getGenre().length() > maxGenreLen) { maxGenreLen = m.getGenre().length(); }
        }
    }
    auto searchEnd = Clock::now();
    double ms = chrono::duration_cast<Ms>(searchEnd - searchStart).count() / 1000.0;
    
    vector<pair<Movie, string>> matches;
    matches.reserve(exactMatches.size() + partialMatches.size());
    matches.insert(matches.end(), exactMatches.begin(), exactMatches.end());
    matches.insert(matches.end(), partialMatches.begin(), partialMatches.end());

    vector<string> lines;
    for (const auto &match : matches) {
        const Movie &m = match.first;
        const string &hTitle = match.second;
        stringstream ss;
        ss << " " << left << setw(maxTitleLen + 2) << hTitle
           << " | " << setw(maxGenreLen + 2) << m.getGenre()
           << " | " << m.getReleaseYear() << "년"
           << " | 평점: " << fixed << setprecision(1) << m.getAverageRating()
           << " (" << m.getRatingCount() << "건)";
        lines.push_back(ss.str());
    }
    
    outQuery = query;
    return {lines, ms};
}

// 전체 영화 목록을 요청된 기준에 따라 정렬하고 포맷팅하여 반환함
pair<vector<string>, double> MovieManager::getSortedMoviesFormatted(int sortChoice, string& outSortName) const {
    auto start = Clock::now();
    vector<string> lines;
    
    if (movies.empty()) {
        auto end = Clock::now();
        double ms = chrono::duration_cast<Ms>(end - start).count() / 1000.0;
        return {lines, ms};
    }
    
    vector<Movie> sortedMovies = movies;
    outSortName = MovieConstants::OPT_SORT[2]; // 기본값: 제목순
    
    switch (sortChoice) {
        case MovieConstants::SORT_RATE:
            outSortName = MovieConstants::OPT_SORT[0];
            sort(sortedMovies.begin(), sortedMovies.end(), [](const Movie &a, const Movie &b) {
                if (a.getAverageRating() != b.getAverageRating()) { return a.getAverageRating() > b.getAverageRating(); }
                return a.getTitle() < b.getTitle();
            });
            break;
        case MovieConstants::SORT_CNT:
            outSortName = MovieConstants::OPT_SORT[1];
            sort(sortedMovies.begin(), sortedMovies.end(), [](const Movie &a, const Movie &b) {
                if (a.getRatingCount() != b.getRatingCount()) { return a.getRatingCount() > b.getRatingCount(); }
                return a.getTitle() < b.getTitle();
            });
            break;
        case MovieConstants::SORT_GENRE:
            outSortName = MovieConstants::OPT_SORT[3];
            sort(sortedMovies.begin(), sortedMovies.end(), [](const Movie &a, const Movie &b) {
                if (a.getGenre() != b.getGenre()) { return a.getGenre() < b.getGenre(); }
                return a.getTitle() < b.getTitle();
            });
            break;
        case MovieConstants::SORT_YEAR:
            outSortName = MovieConstants::OPT_SORT[4];
            sort(sortedMovies.begin(), sortedMovies.end(), [](const Movie &a, const Movie &b) {
                if (a.getReleaseYear() != b.getReleaseYear()) { return a.getReleaseYear() > b.getReleaseYear(); }
                return a.getTitle() < b.getTitle();
            });
            break;
        case MovieConstants::SORT_TITLE:
        default:
            sort(sortedMovies.begin(), sortedMovies.end(), [](const Movie &a, const Movie &b) {
                return a.getTitle() < b.getTitle();
            });
            break;
    }
    
    size_t maxTitleLen = 0, maxGenreLen = 0;
    for (const auto &m : sortedMovies) {
        if (m.getTitle().length() > maxTitleLen) { maxTitleLen = m.getTitle().length(); }
        if (m.getGenre().length() > maxGenreLen) { maxGenreLen = m.getGenre().length(); }
    }
    
    for (const auto &m : sortedMovies) {
        stringstream ss;
        ss << " " << left << setw(maxTitleLen + 2) << m.getTitle()
           << " | " << setw(maxGenreLen + 2) << m.getGenre()
           << " | " << m.getReleaseYear() << "년"
           << " | 평점: " << fixed << setprecision(1) << m.getAverageRating()
           << " (" << m.getRatingCount() << "건)";
        lines.push_back(ss.str());
    }
    
    auto end = Clock::now();
    double ms = chrono::duration_cast<Ms>(end - start).count() / 1000.0;
    
    return {lines, ms};
}

// 제목으로 특정 영화 포인터 검색 (외부에서 영화 유무 확인 등을 위함)
Movie* MovieManager::findMovieByTitle(const string &title) {
    string titleLower = Base::normalizeString(title);
    for (auto &m : movies) {
        if (Base::normalizeString(m.getTitle()) == titleLower) {
            return &m;
        }
    }
    return nullptr;
}

const Movie* MovieManager::findMovieByTitle(const string &title) const {
    string titleLower = Base::normalizeString(title);
    for (const auto &m : movies) {
        if (Base::normalizeString(m.getTitle()) == titleLower) {
            return &m;
        }
    }
    return nullptr;
}


// 파일에서 영화 정보를 메모리로 불러오기 위함
double MovieManager::loadFromFile() {
    movies.clear();
    auto start = Clock::now();
    try {
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "파일을 열 수 없습니다: " << filePath << "\n";
            return 0.0;
        }
        string line;
        getline(file, line);
        while (getline(file, line)) {
            if (line.empty()) { continue; }
            stringstream ss(line);
            string title, genre, yearStr, totalRatingStr, ratingCountStr;
            getline(ss, title, ',');
            getline(ss, genre, ',');
            getline(ss, yearStr, ',');
            getline(ss, totalRatingStr, ',');
            getline(ss, ratingCountStr, ',');
            if (!title.empty()) {
                int year = stoi(yearStr);
                double totalRating = stod(totalRatingStr);
                int ratingCount = stoi(ratingCountStr);
                movies.push_back(Movie(title, year, genre, totalRating, ratingCount));
            }
        }
        file.close();
    } catch (const exception &e) {
        cerr << "loadFromFile 예외 발생: " << e.what() << "\n";
    }
    auto end = Clock::now();
    return chrono::duration_cast<Ms>(end - start).count() / 1000.0;
}

// 메모리의 영화 정보를 파일에 안전하게 보관하기 위함
double MovieManager::saveToFile() {
    auto start = Clock::now();
    try {
        ofstream file(filePath);
        if (!file.is_open()) {
            cerr << "파일을 저장할 수 없습니다: " << filePath << "\n";
            return 0.0;
        }
        file << "title,genre,year,totalRating,ratingCount\n";
        for (const auto &m : movies) {
            file << m.getTitle() << "," << m.getGenre() << "," << m.getReleaseYear() << ","
                 << (m.getAverageRating() * m.getRatingCount()) << "," << m.getRatingCount() << "\n";
        }
        file.close();
    } catch (const exception &e) {
        cerr << "saveToFile 예외 발생: " << e.what() << "\n";
    }
    auto end = Clock::now();
    return chrono::duration_cast<Ms>(end - start).count() / 1000.0;
}

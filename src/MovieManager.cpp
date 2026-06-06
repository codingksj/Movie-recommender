// 영화 CRUD, 검색·정렬 및 CSV 영속화

#include "MovieManager.h"
#include "MovieConstant.h"

using namespace MC::Sort;
#include <iostream>
#include <exception>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iterator>

using namespace std;
using namespace MC::Ui;
using Clock = chrono::high_resolution_clock;
using Ms = chrono::microseconds;

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

pair<vector<string>, double> MovieManager::getSortedMoviesFormatted(int choice, string& sortName) const {
    auto start = Clock::now();
    vector<string> lines;
    
    if (movies.empty()) {
        auto end = Clock::now();
        double ms = chrono::duration_cast<Ms>(end - start).count() / 1000.0;
        return {lines, ms};
    }
    
    vector<Movie> sortedMovies = movies;
    sortName = OPTIONS[TITLE - 1];

    switch (choice) {
        case RATE:
            sortName = OPTIONS[0];
            sort(sortedMovies.begin(), sortedMovies.end(), [](const Movie &a, const Movie &b) {
                if (a.getAverageRating() != b.getAverageRating()) { return a.getAverageRating() > b.getAverageRating(); }
                return a.getTitle() < b.getTitle();
            });
            break;
        case CNT:
            sortName = OPTIONS[1];
            sort(sortedMovies.begin(), sortedMovies.end(), [](const Movie &a, const Movie &b) {
                if (a.getRatingCount() != b.getRatingCount()) { return a.getRatingCount() > b.getRatingCount(); }
                return a.getTitle() < b.getTitle();
            });
            break;
        case GENRE:
            sortName = OPTIONS[3];
            sort(sortedMovies.begin(), sortedMovies.end(), [](const Movie &a, const Movie &b) {
                if (a.getGenre() != b.getGenre()) { return a.getGenre() < b.getGenre(); }
                return a.getTitle() < b.getTitle();
            });
            break;
        case YEAR:
            sortName = OPTIONS[4];
            sort(sortedMovies.begin(), sortedMovies.end(), [](const Movie &a, const Movie &b) {
                if (a.getReleaseYear() != b.getReleaseYear()) { return a.getReleaseYear() > b.getReleaseYear(); }
                return a.getTitle() < b.getTitle();
            });
            break;
        case TITLE:
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

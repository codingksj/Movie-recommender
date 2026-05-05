#include "MovieManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::cin;
using std::string;
using std::vector;

void MovieManager::addMovie() {
    string title, genre;
    int year;

    cout << "영화 제목: ";
    cin.ignore();
    if (!std::getline(cin, title) || title.empty()) {
        return;
    }

    cout << "장르: ";
    std::getline(cin, genre);

    cout << "출시 연도: ";
    if (!(cin >> year)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "올바른 연도(숫자)를 입력해주세요.\n";
        return;
    }

    movies.push_back(Movie(title, year, genre));
    cout << "영화가 추가되었습니다. [제목: " << title << ", 연도: " << year << ", 장르: " << genre << "]\n";
}

void MovieManager::searchByTitle() {
    string title;
    cout << "검색할 영화 제목: ";
    cin.ignore();
    std::getline(cin, title);

    bool found = false;
    Movie target(title);

    for (const auto& m : movies) {
        if (m == target) {
            cout << m << '\n';
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "[" << title << "] 제목의 영화를 찾을 수 없습니다.\n";
    }
}

void MovieManager::printAllMovies() {
    if (movies.empty()) {
        cout << "등록된 영화가 없습니다.\n";
        return;
    }

    cout << "\n#### 전체 영화 목록 ####\n";
    for (const auto& m : movies) {
        cout << m << '\n';
    }
}

void MovieManager::printSortedByRating() {
    if (movies.empty()) {
        cout << "등록된 영화가 없습니다.\n";
        return;
    }

    vector<Movie> sortedMovies = movies;
    std::sort(sortedMovies.begin(), sortedMovies.end(), std::greater<Movie>());

    cout << "\n#### 평점순 영화 목록 ####\n";
    for (const auto& m : sortedMovies) {
        cout << m << '\n';
    }
}

Movie* MovieManager::findMovieByTitle(const string& title) {
    Movie target(title);

    for (auto& m : movies) {
        if (m == target) {
            return &m;
        }
    }

    return nullptr;
}

void MovieManager::loadMovies(const string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        cout << "파일을 열 수 없습니다: " << filename << "\n";
        return;
    }

    string line;
    std::getline(file, line); // 헤더 스킵 (title,genre,year,totalRating,ratingCount)
    
    int count = 0;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        string title, genre, yearStr, totalRatingStr, ratingCountStr;

        std::getline(ss, title, ',');
        std::getline(ss, genre, ',');
        std::getline(ss, yearStr, ',');
        std::getline(ss, totalRatingStr, ',');
        std::getline(ss, ratingCountStr, ',');

        if (!title.empty()) {
            movies.push_back(Movie(title, std::stoi(yearStr), genre, std::stod(totalRatingStr), std::stoi(ratingCountStr)));
            count++;
        }
    }
    file.close();
    cout << "영화 데이터 로드 완료 (" << count << "건)\n";
}

void MovieManager::saveMovies(const string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        cout << "파일을 저장할 수 없습니다: " << filename << "\n";
        return;
    }

    file << "title,genre,year,totalRating,ratingCount\n";
    for (const auto& m : movies) {
        file << m.getTitle() << "," 
             << m.getGenre() << "," 
             << m.getReleaseYear() << "," 
             << (m.getAverageRating() * m.getRatingCount()) << "," // totalRating 복원
             << m.getRatingCount() << "\n";
    }
    file.close();
    cout << "영화 데이터 저장 완료\n";
}

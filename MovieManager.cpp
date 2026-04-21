#include "MovieManager.h"
#include <iostream>
#include <limits>

using std::cout;
using std::cin;
using std::string;

void MovieManager::addMovie() {
    string title, genre;
    int year;

    cout << "영화 제목: ";
    cin.ignore();
    std::getline(cin, title);
    cout << "장르: ";
    std::getline(cin, genre);
    cout << "출시 연도: ";
    cin >> year;

    movies.push_back(Movie(title, genre, year));
    cout << "영화가 추가되었습니다.\n";
}

void MovieManager::searchByTitle() {
    string title;
    cout << "검색할 영화 제목: ";
    cin.ignore();
    std::getline(cin, title);

    bool found = false;
    for (const auto& m : movies) {
        if (m.getTitle().find(title) != string::npos) {
            cout << m << '\n';
            found = true;
        }
    }
    if (!found) cout << "해당 제목의 영화를 찾을 수 없습니다.\n";
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
    std::vector<Movie> sortedMovies = movies;
    std::sort(sortedMovies.begin(), sortedMovies.end(), [](const Movie& a, const Movie& b) {
        return a.getAverageRating() > b.getAverageRating();
    });

    cout << "\n#### 평점순 영화 목록 ####\n";
    for (const auto& m : sortedMovies) {
        cout << m << '\n';
    }
}

Movie* MovieManager::findMovieByTitle(const string& title) {
    for (auto& m : movies) {
        if (m.getTitle() == title) {
            return &m;
        }
    }
    return nullptr;
}

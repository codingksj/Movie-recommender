#include "MovieManager.h"
#include <iostream>
using std::cout;
using std::cin;
using std::string;
using std::vector;
void MovieManager::addMovie() {
    string title, genre;
    int year;
    cout << "영화 제목: ";
    cin.ignore();
    if (!std::getline(cin, title) || title.empty()) return;
    cout << "장르: ";
    std::getline(cin, genre);
    cout << "출시 연도: ";
    if (!(cin >> year)) {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "올바른 연도(숫자)를 입력해주세요.\n";
        return;
    }
    movies.push_back(Movie(title, genre, year));
    cout << "영화가 추가되었습니다.\n";
}
void MovieManager::searchByTitle() {
    string title;
    cout << "검색할 영화 제목: ";
    cin.ignore();
    std::getline(cin, title);
    bool found = false;
    Movie target(title, "temp", 2000);
    for (const auto& m : movies) {
        if (m == target) {
            cout << m << '\n';
            found = true;
            break;
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
    for (const auto& m : movies) cout << m << '\n';
}
void MovieManager::printSortedByRating() {
    if (movies.empty()) {
        cout << "등록된 영화가 없습니다.\n";
        return;
    }
    vector<Movie> sortedMovies = movies;
    std::sort(sortedMovies.begin(), sortedMovies.end());
    cout << "\n#### 평점순 영화 목록 ####\n";
    for (const auto& m : sortedMovies) cout << m << '\n';
}
Movie* MovieManager::findMovieByTitle(const string& title) {
    Movie target(title, "temp", 2000);
    for (auto& m : movies) {
        if (m == target) return &m;
    }
    return nullptr;
}

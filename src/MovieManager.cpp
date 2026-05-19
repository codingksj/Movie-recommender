// 영화 목록을 추가, 검색 및 관리하는 클래스

#include "MovieManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "menu.h"

using std::cout;
using std::cin;
using std::string;
using std::vector;

// 정렬 관련 서브메뉴 상수
const string TITLE_SORT = "정렬 기준 선택";
const vector<string> OPTIONS_SORT = {
    "평점 높은순",
    "평점 많은순",
    "제목 사전순",
    "장르 사전순",
    "개봉연도순 (최신순)"
};

// 정렬 선택 번호 상수
const int SORT_RATE = 1;
const int SORT_CNT = 2;
const int SORT_TITLE = 3;
const int SORT_GENRE = 4;
const int SORT_YEAR = 5;

// 신규 영화 추가 프롬프트 처리
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

// 영화 제목 검색 프롬프트 처리
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

// 전체 영화 목록 정렬 및 출력
void MovieManager::printAllMovies() {
    if (movies.empty()) {
        cout << "등록된 영화가 없습니다.\n";
        return;
    }

    Menu::showSubMenu(TITLE_SORT, OPTIONS_SORT);
    int sortChoice = Menu::getChoice();
    if (sortChoice <= 0 || sortChoice > SORT_YEAR) {
        sortChoice = SORT_TITLE;
    }

    vector<Movie> sortedMovies = movies;

    string sortName = "제목 사전순";
    switch (sortChoice) {
        case SORT_RATE: {
            sortName = "평점 높은순";
            std::sort(sortedMovies.begin(), sortedMovies.end(), [](const Movie& a, const Movie& b) {
                if (a.getAverageRating() != b.getAverageRating()) {
                    return a.getAverageRating() > b.getAverageRating();
                }
                return a.getTitle() < b.getTitle();
            });
            break;
        }
        case SORT_CNT: {
            sortName = "평점 많은순";
            std::sort(sortedMovies.begin(), sortedMovies.end(), [](const Movie& a, const Movie& b) {
                if (a.getRatingCount() != b.getRatingCount()) {
                    return a.getRatingCount() > b.getRatingCount();
                }
                return a.getTitle() < b.getTitle();
            });
            break;
        }
        case SORT_GENRE: {
            sortName = "장르 사전순";
            std::sort(sortedMovies.begin(), sortedMovies.end(), [](const Movie& a, const Movie& b) {
                if (a.getGenre() != b.getGenre()) {
                    return a.getGenre() < b.getGenre();
                }
                return a.getTitle() < b.getTitle();
            });
            break;
        }
        case SORT_YEAR: {
            sortName = "개봉연도순 (최신순)";
            std::sort(sortedMovies.begin(), sortedMovies.end(), [](const Movie& a, const Movie& b) {
                if (a.getReleaseYear() != b.getReleaseYear()) {
                    return a.getReleaseYear() > b.getReleaseYear();
                }
                return a.getTitle() < b.getTitle();
            });
            break;
        }
        case SORT_TITLE:
        default: {
            std::sort(sortedMovies.begin(), sortedMovies.end(), [](const Movie& a, const Movie& b) {
                return a.getTitle() < b.getTitle();
            });
            break;
        }
    }

    size_t maxTitleLen = 0;
    size_t maxGenreLen = 0;
    for (const auto& m : sortedMovies) {
        if (m.getTitle().length() > maxTitleLen) {
            maxTitleLen = m.getTitle().length();
        }
        if (m.getGenre().length() > maxGenreLen) {
            maxGenreLen = m.getGenre().length();
        }
    }

    vector<string> lines;
    for (const auto& m : sortedMovies) {
        std::stringstream ss;
        ss << " " << std::left << std::setw(maxTitleLen + 2) << m.getTitle() 
           << " | " << std::setw(maxGenreLen + 2) << m.getGenre() 
           << " | " << m.getReleaseYear() << "년"
           << " | 평점: " << std::fixed << std::setprecision(1) << m.getAverageRating() 
           << " (" << m.getRatingCount() << "건)";
        lines.push_back(ss.str());
    }

    Menu::showDynamicResult("전체 영화 목록 (정렬: " + sortName + ")", lines);
}

// 제목으로 특정 영화 포인터 검색
Movie* MovieManager::findMovieByTitle(const string& title) {
    Movie target(title);

    for (auto& m : movies) {
        if (m == target) {
            return &m;
        }
    }

    return nullptr;
}

// 파일에서 영화 정보 불러오기
void MovieManager::loadFromFile() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        cout << "파일을 열 수 없습니다: " << filePath << "\n";
        return;
    }

    string line;
    std::getline(file, line);
    
    int count = 0;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
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

// 파일로 영화 정보 저장하기
void MovieManager::saveToFile() {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        cout << "파일을 저장할 수 없습니다: " << filePath << "\n";
        return;
    }

    file << "title,genre,year,totalRating,ratingCount\n";
    for (const auto& m : movies) {
        file << m.getTitle() << "," 
             << m.getGenre() << "," 
             << m.getReleaseYear() << "," 
             << (m.getAverageRating() * m.getRatingCount()) << ","
             << m.getRatingCount() << "\n";
    }
    file.close();
    cout << "영화 데이터 저장 완료\n";
}

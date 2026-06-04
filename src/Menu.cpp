// 사용자 입출력 및 메뉴 제어를 분리하여 단일 책임 원칙을 준수하기 위함

#include "Menu.h"
#include "MovieConstant.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

// 결과 리스트의 각 항목에 인덱스 번호를 붙여 포맷팅
vector<string> formatList(const vector<string>& items) {
    vector<string> lines;
    for (size_t i = 0; i < items.size(); ++i) {
        lines.push_back("  " + to_string(i + 1) + ". " + items[i]);
    }
    return lines;
}

// 상단 타이틀 배너 출력
void Menu::showHeader(const string& title) {
    cout << "\n========================================\n";
    cout << "       " << title << "\n";
    cout << "========================================\n";
}

void Menu::showMainMenu() {
    showHeader(MovieConstants::TITLE_MAIN);
    for (size_t i = 0; i < MovieConstants::OPTIONS_MAIN.size(); ++i) {
        cout << "  [" << (i + 1) << "] " << MovieConstants::OPTIONS_MAIN[i] << "\n";
    }
    cout << "  [0] 종료 및 저장\n";
    cout << "========================================\n";
    cout << "선택> ";
}

void Menu::showSubMenu(const string& title, const vector<string>& options) {
    showHeader(title);
    for (size_t i = 0; i < options.size(); ++i) {
        cout << "  [" << (i + 1) << "] " << options[i] << "\n";
    }
    cout << "  [0] 이전 메뉴로\n";
    cout << "========================================\n";
    cout << "선택> ";
}

// 다이나믹 너비 기준 결과 리스트 출력
void Menu::showDynamicResult(const string& title, const vector<string>& lines) {
    size_t max_len = title.length() + MovieConstants::TITLE_PAD;
    for (const auto& line : lines) {
        if (line.length() > max_len) {
            max_len = line.length();
        }
    }

    if (max_len < MovieConstants::MIN_WIDTH) {
        max_len = MovieConstants::MIN_WIDTH;
    }

    string border(max_len + 4, '=');
    cout << "\n" << border << "\n";
    
    int padding = (max_len + 4 - title.length()) / 2;
    cout << string(padding, ' ') << title << "\n";
    cout << border << "\n";

    for (const auto& line : lines) {
        cout << "  " << line << "\n";
    }
    cout << border << "\n";
}

int Menu::getChoice() {
    int choice;
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(MovieConstants::IGNORE_LIMIT, '\n');
        return -1;
    }
    return choice;
}

// 영화 관리 메뉴 처리 루프
void Menu::handleMovieMenu(MovieManager& movieManager) {
    while (true) {
        showSubMenu(MovieConstants::TITLE_MOVIE, MovieConstants::OPTIONS_MOVIE);
        int subChoice = getChoice();
        if (subChoice == 0) { break; }

        if (subChoice == 1) { promptAddMovie(movieManager); }
        else if (subChoice == 2) { promptSearchMovie(movieManager); }
        else if (subChoice == 3) { promptPrintMovies(movieManager); }
        else { cout << "잘못된 선택입니다.\n"; }
    }
}

void Menu::promptAddMovie(MovieManager& movieManager) {
    string title, genre;
    int year;
    cout << "영화 제목: ";
    cin.ignore();
    if (!getline(cin, title) || title.empty()) { return; }
    
    cout << "장르: ";
    getline(cin, genre);
    
    cout << "출시 연도: ";
    if (!(cin >> year)) {
        cin.clear();
        cin.ignore(MovieConstants::IGNORE_LIMIT, '\n');
        cout << "올바른 연도(숫자)를 입력해주세요.\n";
        return;
    }
    
    auto res = movieManager.addMovie(title, year, genre);
    if (res.first == MovieResult::DUPLICATE_MOVIE) {
        cout << "이미 존재하는 영화입니다.\n";
    } else {
        cout << "영화가 추가되었습니다. [제목: " << title << ", 연도: " << year << ", 장르: " << genre << "]\n";
    }
    cout << "addMovie took " << fixed << setprecision(3) << res.second << " ms\n";
}

void Menu::promptSearchMovie(MovieManager& movieManager) {
    string query;
    cout << "검색할 영화 제목: ";
    cin.ignore();
    if (!getline(cin, query) || query.empty()) { return; }
    
    string outQuery;
    auto res = movieManager.searchByTitleFormatted(query, outQuery);
    
    if (res.first.empty()) {
        cout << "[" << outQuery << "] 제목의 영화를 찾을 수 없습니다.\n";
    } else {
        showDynamicResult("영화 검색 결과 (" + outQuery + ")", res.first);
    }
    cout << "search core took " << fixed << setprecision(3) << res.second << " ms\n";
}

void Menu::promptPrintMovies(MovieManager& movieManager) {
    if (movieManager.getMovies().empty()) {
        cout << "등록된 영화가 없습니다.\n";
        return;
    }
    showSubMenu("정렬 기준 선택", MovieConstants::OPTIONS_SORT);
    int sortChoice = getChoice();
    if (sortChoice <= 0 || sortChoice > 5) { sortChoice = 3; } // 기본값: 제목순
    
    string sortName;
    auto res = movieManager.getSortedMoviesFormatted(sortChoice, sortName);
    showDynamicResult("전체 영화 목록 (정렬: " + sortName + ")", res.first);
    cout << "printAllMovies took " << fixed << setprecision(3) << res.second << " ms\n";
}

// 사용자 관리 메뉴 처리 루프
void Menu::handleUserMenu(UserManager& userManager) {
    while (true) {
        showSubMenu(MovieConstants::TITLE_USER, MovieConstants::OPTIONS_USER);
        int subChoice = getChoice();
        if (subChoice == 0) { break; }

        if (subChoice == 1) { promptAddUser(userManager); }
        else if (subChoice == 2) { promptPrintUsers(userManager); }
        else { cout << "잘못된 선택입니다.\n"; }
    }
}

void Menu::promptAddUser(UserManager& userManager) {
    string name, email;
    cout << "사용자 이름: ";
    cin.ignore();
    if (!getline(cin, name) || name.empty()) { return; }
    
    cout << "사용자 이메일: ";
    if (!getline(cin, email) || email.empty()) { return; }
    
    auto res = userManager.addUser(name, email);
    if (res.first == UserResult::DUPLICATE_USER) {
        cout << "[" << name << ", " << email << "]은(는) 이미 존재하는 사용자입니다.\n";
    } else {
        cout << "사용자가 추가되었습니다. [이름: " << name << ", 이메일: " << email << "]\n";
    }
    cout << "addUser took " << fixed << setprecision(3) << res.second << " ms\n";
}

void Menu::promptPrintUsers(UserManager& userManager) {
    if (userManager.getUsers().empty()) {
        cout << "등록된 사용자가 없습니다.\n";
        return;
    }
    vector<string> lines = userManager.getAllUsersFormatted();
    showDynamicResult("전체 사용자 목록", lines);
}

// 평점 관리 메뉴 처리 루프
void Menu::handleRatingMenu(RatingManager& ratingManager, MovieManager& movieManager, UserManager& userManager) {
    while (true) {
        showSubMenu(MovieConstants::TITLE_RATING, MovieConstants::OPTIONS_RATING);
        int subChoice = getChoice();
        if (subChoice == 0) { break; }

        if (subChoice == 1) { promptAddRating(ratingManager, movieManager, userManager); }
        else if (subChoice == 2) { promptPrintRatings(ratingManager); }
        else { cout << "잘못된 선택입니다.\n"; }
    }
}

void Menu::promptAddRating(RatingManager& ratingManager, MovieManager& movieManager, UserManager& userManager) {
    string userName, movieTitle;
    double score;
    
    cout << "사용자 이름: ";
    cin.ignore();
    if (!getline(cin, userName) || userName.empty()) { return; }
    
    cout << "영화 제목: ";
    if (!getline(cin, movieTitle) || movieTitle.empty()) { return; }
    
    cout << "평점 (" << MovieConstants::MIN_RATE << " ~ " << MovieConstants::MAX_RATE << "): ";
    if (!(cin >> score)) {
        cin.clear();
        cin.ignore(MovieConstants::IGNORE_LIMIT, '\n');
        return;
    }
    
    auto res = ratingManager.addRating(userName, movieTitle, score, movieManager, userManager);
    if (res.first == RatingResult::USER_NOT_FOUND) {
        cout << "[" << userName << "]은(는) 등록되지 않은 사용자입니다.\n";
    } else if (res.first == RatingResult::MOVIE_NOT_FOUND) {
        cout << "[" << movieTitle << "] 제목의 영화를 찾을 수 없습니다.\n";
    } else if (res.first == RatingResult::INVALID_SCORE) {
        cout << "올바른 범위의 평점을 입력해주세요.\n";
    } else {
        cout << "평점이 등록되었습니다. [사용자: " << userName << ", 영화: " << movieTitle << ", 평점: " << score << "]\n";
    }
    cout << "addRating took " << fixed << setprecision(3) << res.second << " ms\n";
}

void Menu::promptPrintRatings(RatingManager& ratingManager) {
    string movieTitle;
    cout << "평점을 볼 영화 제목: ";
    cin.ignore();
    getline(cin, movieTitle);
    
    vector<string> lines = ratingManager.getRatingsByMovieFormatted(movieTitle);
    if (lines.empty()) {
        cout << "등록된 평점이 없습니다.\n";
    } else {
        showDynamicResult("[" + movieTitle + "] 평점 목록", lines);
    }
}

// 추천 서비스 메뉴 처리 루프
void Menu::handleRecommendationMenu(Recommender& recommender) {
    while (true) {
        showSubMenu(MovieConstants::TITLE_RECOMMEND, MovieConstants::OPTIONS_RECOMMEND);
        int subChoice = getChoice();
        if (subChoice == 0) { break; }

        if (subChoice == 1) { promptSimilarUsers(recommender); }
        else if (subChoice == 2) { promptRecommendByUser(recommender); }
        else if (subChoice == 3) { promptRecommendByGenre(recommender); }
        else { cout << "잘못된 선택입니다.\n"; }
    }
}

void Menu::promptSimilarUsers(Recommender& recommender) {
    string targetUser;
    cout << "유사 사용자를 찾을 사용자 이름 입력: ";
    cin >> targetUser;
    
    if (!recommender.getUserManager().findUserByName(targetUser)) {
        cout << "존재하지 않는 사용자입니다.\n";
        return;
    }
    
    if (recommender.getUserRatings(targetUser).empty()) {
        cout << "해당 사용자의 평점 이력이 없어 유사도를 계산할 수 없습니다.\n";
        return;
    }
    
    auto similarities = recommender.getSimilarUsers(targetUser, MovieConstants::SIM_LIMIT);
    if (similarities.empty()) {
        cout << "유사한 사용자가 없습니다.\n";
        return;
    }
    
    vector<string> lines;
    int count = 0;
    for (const auto& s : similarities) {
        stringstream ss;
        ss << "  " << ++count << ". " << s.first << " (유사도: " << fixed << setprecision(3) << s.second << ")";
        lines.push_back(ss.str());
    }
    showDynamicResult(targetUser + "와 유사한 사용자 상위 " + to_string(MovieConstants::SIM_LIMIT) + "명", lines);
}

void Menu::promptRecommendByUser(Recommender& recommender) {
    string targetUser;
    cout << "추천을 받을 사용자 이름 입력: ";
    cin >> targetUser;
    
    if (!recommender.getUserManager().findUserByName(targetUser)) {
        cout << "존재하지 않는 사용자입니다.\n";
        return;
    }
    
    vector<string> recommended = recommender.recommend(targetUser, MovieConstants::COLLAB_K, MovieConstants::COLLAB_N);
    if (recommended.empty()) {
        cout << "추천할 만한 새로운 영화가 없습니다.\n";
        return;
    }
    showDynamicResult(targetUser + "님을 위한 추천 영화", formatList(recommended));
}

void Menu::promptRecommendByGenre(Recommender& recommender) {
    string title;
    cout << "기준 영화 제목 입력: ";
    cin.ignore();
    getline(cin, title);
    
    vector<string> recommended = recommender.recommendByGenre(title, MovieConstants::GENRE_N);
    if (recommended.empty()) {
        cout << "추천할 비슷한 장르의 영화가 없거나 존재하지 않는 영화입니다.\n";
        return;
    }
    showDynamicResult(title + "와(과) 비슷한 장르 추천 영화", formatList(recommended));
}

// 데이터 로드/세이브 요약
void Menu::showLoadSummary(int movieCount, int userCount, int ratingCount) {
    cout << "\n========================================\n";
    cout << "          데이터 로드 완료\n";
    cout << "========================================\n";
    cout << "  영화 데이터     : " << movieCount << "건\n";
    cout << "  사용자 데이터   : " << userCount << "건\n";
    cout << "  평점 데이터     : " << ratingCount << "건\n";
    cout << "========================================\n";
}

void Menu::showSaveSummary(int movieCount, int userCount, int ratingCount) {
    cout << "\n========================================\n";
    cout << "       데이터 저장 및 프로그램 종료\n";
    cout << "========================================\n";
    cout << "  영화 데이터     : " << movieCount << "건 저장 완료\n";
    cout << "  사용자 데이터   : " << userCount << "건 저장 완료\n";
    cout << "  평점 데이터     : " << ratingCount << "건 저장 완료\n";
    cout << "========================================\n";
}

// 통계 메뉴 핸들러
void Menu::handleStatisticsMenu(Statistics& statistics) {
    while (true) {
        showSubMenu(MovieConstants::TITLE_STATISTICS, MovieConstants::OPTIONS_STATISTICS);
        int subChoice = getChoice();
        if (subChoice == 0) { break; }

        if (subChoice == 1) { promptOverallStatistics(statistics); }
        else if (subChoice == 2) { promptTopMoviesByRating(statistics); }
        else if (subChoice == 3) { promptTopMoviesByRatingCount(statistics); }
        else if (subChoice == 4) { promptStatisticsByGenre(statistics); }
        else if (subChoice == 5) { promptStatisticsByYear(statistics); }
        else if (subChoice == 6) { promptTopUsersByRatingCount(statistics); }
        else if (subChoice == 7) { promptUserStatistics(statistics); }
        else { cout << "잘못된 선택입니다.\n"; }
    }
}

void Menu::promptOverallStatistics(Statistics& statistics) {
    auto stats = statistics.getOverallStatistics();
    showDynamicResult("전체 통계", stats);
}

void Menu::promptTopMoviesByRating(Statistics& statistics) {
    auto topMovies = statistics.getTopMoviesByRating(10);
    showDynamicResult("평점 높은 순서 (상위 10)", formatList(topMovies));
}

void Menu::promptTopMoviesByRatingCount(Statistics& statistics) {
    auto topMovies = statistics.getTopMoviesByRatingCount(10);
    showDynamicResult("평가 많은 순서 (상위 10)", formatList(topMovies));
}

void Menu::promptStatisticsByGenre(Statistics& statistics) {
    auto genreStats = statistics.getStatisticsByGenre();
    showDynamicResult("장르별 평균 평점", genreStats);
}

void Menu::promptStatisticsByYear(Statistics& statistics) {
    auto yearStats = statistics.getStatisticsByYear();
    showDynamicResult("연도별 평균 평점", yearStats);
}

void Menu::promptTopUsersByRatingCount(Statistics& statistics) {
    auto topUsers = statistics.getTopUsersByRatingCount(10);
    showDynamicResult("사용자별 평가 수 (상위 10)", formatList(topUsers));
}

void Menu::promptUserStatistics(Statistics& statistics) {
    string userName;
    cout << "사용자 이름 입력: ";
    cin.ignore();
    getline(cin, userName);
    
    auto userStats = statistics.getUserStatistics(userName);
    showDynamicResult(userName + "님의 통계", userStats);
}

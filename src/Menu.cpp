#include "Menu.h"
#include "MovieConstant.h"
#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include "Recommender.h"
#include "Statistics.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <limits>
#include <cmath>

using namespace std;

namespace {
    void printMenu(const string& title, const vector<string>& options, const string& exitText) {
        Menu::header(title);
        int numWidth = static_cast<int>(to_string(options.size()).length());
        for (size_t i = 0; i < options.size(); ++i) {
            cout << "  [" << setw(numWidth) << right << (i + 1) << "] " << options[i] << "\n";
        }
        cout << "  [" << setw(numWidth) << right << 0 << "] " << exitText << "\n";
        cout << "========================================\n";
        cout << "선택> ";
    }
}

// Helper to format list
static vector<string> formatList(const vector<string>& items) {
    vector<string> lines;
    int numWidth = static_cast<int>(to_string(items.size()).length());
    for (size_t i = 0; i < items.size(); ++i) {
        stringstream ss;
        ss << "  " << setw(numWidth) << right << (i + 1) << ". " << items[i];
        lines.push_back(ss.str());
    }
    return lines;
}

// UI helpers (unchanged)
void Menu::header(const string& title) {
    cout << "\n========================================\n";
    cout << "       " << title << "\n";
    cout << "========================================\n";
}

void Menu::mainMenu() {
    printMenu(MovieConstants::TITLE_MAIN, MovieConstants::OPT_MAIN, "종료 및 저장");
}

void Menu::subMenu(const string& title, const vector<string>& options) {
    printMenu(title, options, "이전 메뉴로");
}

void Menu::resultBox(const string& title, const vector<string>& lines) {
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

string Menu::readLine() {
    string line;
    getline(cin, line);
    return line;
}

int Menu::readChoice() {
    string input = readLine();
    if (input.empty()) {
        return -1;
    }
    stringstream ss(input);
    int choice;
    if (ss >> choice) {
        return choice;
    }
    return -1;
}

double Menu::readDouble() {
    string input = readLine();
    if (input.empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    stringstream ss(input);
    double value;
    if (ss >> value) {
        return value;
    }
    return std::numeric_limits<double>::quiet_NaN();
}

// ==================== MovieHandler ====================
void Menu::MovieHandler::add(MovieManager& manager) {
    cout << "영화 제목: ";
    string title = readLine();
    if (title.empty()) {
        return;
    }
    cout << "장르: ";
    string genre = readLine();
    cout << "출시 연도: ";
    string yearInput = readLine();
    stringstream ss(yearInput);
    int year;
    if (!(ss >> year)) {
        cout << "올바른 연도(숫자)를 입력해주세요.\n";
        return;
    }
    auto res = manager.addMovie(title, year, genre);
    if (res.first == MovieResult::DUPLICATE_MOVIE) {
        cout << "이미 존재하는 영화입니다.\n";
    } else {
        cout << "영화가 추가되었습니다. [제목: " << title << ", 연도: " << year << ", 장르: " << genre << "]\n";
    }
    cout << "addMovie took " << fixed << setprecision(3) << res.second << " ms\n";
}

void Menu::MovieHandler::search(MovieManager& manager) {
    cout << "검색할 영화 제목: ";
    string query = readLine();
    if (query.empty()) {
        return;
    }
    string outQuery;
    auto res = manager.searchByTitleFormatted(query, outQuery);
    if (res.first.empty()) {
        cout << "[" << outQuery << "] 제목의 영화를 찾을 수 없습니다.\n";
    } else {
        resultBox("영화 검색 결과 (" + outQuery + ")", res.first);
    }
    cout << "search core took " << fixed << setprecision(3) << res.second << " ms\n";
}

void Menu::MovieHandler::print(MovieManager& manager) {
    if (manager.getMovies().empty()) {
        cout << "등록된 영화가 없습니다.\n";
        return;
    }
    subMenu("정렬 기준 선택", MovieConstants::OPT_SORT);
    int sortChoice = readChoice();
    if (sortChoice <= 0 || sortChoice > MovieConstants::MAX_SORT_OPTION) {
        sortChoice = MovieConstants::DEFAULT_SORT_CHOICE; 
    }
    string sortName;
    auto res = manager.getSortedMoviesFormatted(sortChoice, sortName);
    resultBox("전체 영화 목록 (정렬: " + sortName + ")", res.first);
    cout << "printAllMovies took " << fixed << setprecision(3) << res.second << " ms\n";
}

// ==================== UserHandler ====================
void Menu::UserHandler::add(UserManager& manager) {
    cout << "사용자 이름: ";
    string name = readLine();
    if (name.empty()) {
        return;
    }
    cout << "사용자 이메일: ";
    string email = readLine();
    if (email.empty()) {
        return;
    }
    auto res = manager.addUser(name, email);
    if (res.first == UserResult::DUPLICATE_USER) {
        cout << "[" << name << ", " << email << "]은(는) 이미 존재하는 사용자입니다.\n";
    } else {
        cout << "사용자가 추가되었습니다. [이름: " << name << ", 이메일: " << email << "]\n";
    }
    cout << "addUser took " << fixed << setprecision(3) << res.second << " ms\n";
}

void Menu::UserHandler::print(UserManager& manager) {
    if (manager.getUsers().empty()) {
        cout << "등록된 사용자가 없습니다.\n";
        return;
    }
    vector<string> lines = manager.getAllUsersFormatted();
    resultBox("전체 사용자 목록", lines);
}

// ==================== RatingHandler ====================
void Menu::RatingHandler::add(RatingManager& rMgr, MovieManager& mMgr, UserManager& uMgr) {
    cout << "사용자 이름: ";
    string userName = readLine();
    if (userName.empty()) {
        return;
    }
    cout << "영화 제목: ";
    string movieTitle = readLine();
    if (movieTitle.empty()) {
        return;
    }
    cout << "평점 (" << MovieConstants::MIN_RATE << " ~ " << MovieConstants::MAX_RATE << "): ";
    double score = readDouble();
    if (std::isnan(score)) {
        cout << "올바른 평점을 입력해주세요.\n";
        return;
    }
    auto res = rMgr.addRating(userName, movieTitle, score, mMgr, uMgr);
    switch (res.first) {
        case RatingResult::USER_NOT_FOUND:
            cout << "[" << userName << "]은(는) 등록되지 않은 사용자입니다.\n";
            break;
        case RatingResult::MOVIE_NOT_FOUND:
            cout << "[" << movieTitle << "] 제목의 영화를 찾을 수 없습니다.\n";
            break;
        case RatingResult::INVALID_SCORE:
            cout << "올바른 범위의 평점을 입력해주세요.\n";
            break;
        case RatingResult::DUPLICATE_RATING:
            cout << "이미 등록된 평점입니다. [사용자: " << userName << ", 영화: " << movieTitle << "]\n";
            break;
        default:
            cout << "평점이 등록되었습니다. [사용자: " << userName << ", 영화: " << movieTitle << ", 평점: " << score << "]\n";
            break;
    }
    cout << "addRating took " << fixed << setprecision(3) << res.second << " ms\n";
}

void Menu::RatingHandler::print(RatingManager& manager) {
    cout << "평점을 볼 영화 제목: ";
    string movieTitle = readLine();
    vector<string> lines = manager.getRatingsByMovieFormatted(movieTitle);
    if (lines.empty()) {
        cout << "등록된 평점이 없습니다.\n";
    } else {
        resultBox("[" + movieTitle + "] 평점 목록", lines);
    }
}

// ==================== RecommendationHandler ====================
void Menu::RecommendationHandler::similarUsers(Recommender& recommender) {
    cout << "유사 사용자를 찾을 사용자 이름 입력: ";
    string targetUser = readLine();
    if (!recommender.getUserManager().findUserByName(targetUser)) {
        cout << "존재하지 않는 사용자입니다.\n";
        return;
    }
    if (recommender.getUserRatings(targetUser).empty()) {
        cout << "해당 사용자의 평점 이력이 없어 유사도를 계산할 수 없습니다.\n";
        return;
    }
    auto sims = recommender.getSimilarUsers(targetUser, MovieConstants::SIM_LIMIT);
    if (sims.empty()) {
        cout << "유사한 사용자가 없습니다.\n";
        return;
    }
    vector<string> lines;
    int count = 0;
    for (const auto& s : sims) {
        stringstream ss;
        ss << "  " << ++count << ". " << s.first << " (유사도: " << fixed << setprecision(3) << s.second << ")";
        lines.push_back(ss.str());
    }
    resultBox(targetUser + "와 유사한 사용자 상위 " + to_string(MovieConstants::SIM_LIMIT) + "명", lines);
}

void Menu::RecommendationHandler::recommendByUser(Recommender& recommender) {
    cout << "추천을 받을 사용자 이름 입력: ";
    string targetUser = readLine();
    if (!recommender.getUserManager().findUserByName(targetUser)) {
        cout << "존재하지 않는 사용자입니다.\n";
        return;
    }
    vector<string> rec = recommender.recommend(targetUser, MovieConstants::COLLAB_K, MovieConstants::COLLAB_N);
    if (rec.empty()) {
        cout << "추천할 만한 새로운 영화가 없습니다.\n";
        return;
    }
    resultBox(targetUser + "님을 위한 추천 영화", formatList(rec));
}

void Menu::RecommendationHandler::recommendByGenre(Recommender& recommender) {
    cout << "기준 영화 제목 입력: ";
    string title = readLine();
    vector<string> rec = recommender.recommendByGenre(title, MovieConstants::GENRE_N);
    if (rec.empty()) {
        cout << "추천할 비슷한 장르의 영화가 없거나 존재하지 않는 영화입니다.\n";
        return;
    }
    resultBox(title + "와(과) 비슷한 장르 추천 영화", formatList(rec));
}

// ==================== StatisticsHandler ====================
void Menu::StatisticsHandler::overall(Statistics& manager) {
    auto stats = manager.getOverallStatistics();
    resultBox("전체 통계", stats);
}

void Menu::StatisticsHandler::topByRating(Statistics& manager) {
    auto top = manager.getTopMoviesByRating(MovieConstants::TOP_N);
    resultBox("평점 높은 순서 (상위 " + std::to_string(MovieConstants::TOP_N) + ")", formatList(top));
}

void Menu::StatisticsHandler::topByRatingCount(Statistics& manager) {
    auto top = manager.getTopMoviesByRatingCount(MovieConstants::TOP_N);
    resultBox("평가 많은 순서 (상위 " + std::to_string(MovieConstants::TOP_N) + ")", formatList(top));
}

void Menu::StatisticsHandler::byGenre(Statistics& manager) {
    auto genre = manager.getStatisticsByGenre();
    resultBox("장르별 평균 평점", genre);
}

void Menu::StatisticsHandler::byYear(Statistics& manager) {
    auto year = manager.getStatisticsByYear();
    resultBox("연도별 평균 평점", year);
}

void Menu::StatisticsHandler::topUsers(Statistics& manager) {
    auto top = manager.getTopUsersByRatingCount(MovieConstants::TOP_N);
    resultBox("사용자별 평가 수 (상위 " + std::to_string(MovieConstants::TOP_N) + ")", formatList(top));
}

void Menu::StatisticsHandler::userStats(Statistics& manager) {
    cout << "사용자 이름 입력: ";
    string userName = readLine();
    auto stats = manager.getUserStatistics(userName);
    resultBox(userName + "님의 통계", stats);
}

// ==================== Menu public handlers ====================
void Menu::handleMovieMenu(MovieManager& movieMgr) {
    while (true) {
        subMenu(MovieConstants::TITLE_MOVIE, MovieConstants::OPT_MOVIE);
        int subChoice = readChoice();
        if (subChoice == MovieConstants::MENU_BACK) {
            break;
        }
        switch (subChoice) {
            case MovieConstants::MOV_ADD:
                MovieHandler::add(movieMgr);
                break;
            case MovieConstants::MOV_SEARCH:
                MovieHandler::search(movieMgr);
                break;
            case MovieConstants::MOV_LIST:
                MovieHandler::print(movieMgr);
                break;
            default:
                cout << "잘못된 선택입니다.\n";
                break;
        }
    }
}

void Menu::handleUserMenu(UserManager& userMgr) {
    while (true) {
        subMenu(MovieConstants::TITLE_USER, MovieConstants::OPT_USER);
        int subChoice = readChoice();
        if (subChoice == MovieConstants::MENU_BACK) {
            break;
        }
        switch (subChoice) {
            case MovieConstants::USR_ADD:
                UserHandler::add(userMgr);
                break;
            case MovieConstants::USR_LIST:
                UserHandler::print(userMgr);
                break;
            default:
                cout << "잘못된 선택입니다.\n";
                break;
        }
    }
}

void Menu::handleRatingMenu(RatingManager& ratingMgr, MovieManager& movieMgr, UserManager& userMgr) {
    while (true) {
        subMenu(MovieConstants::TITLE_RATING, MovieConstants::OPT_RATING);
        int subChoice = readChoice();
        if (subChoice == MovieConstants::MENU_BACK) {
            break;
        }
        switch (subChoice) {
            case MovieConstants::RAT_ADD:
                RatingHandler::add(ratingMgr, movieMgr, userMgr);
                break;
            case MovieConstants::RAT_LIST:
                RatingHandler::print(ratingMgr);
                break;
            default:
                cout << "잘못된 선택입니다.\n";
                break;
        }
    }
}

void Menu::handleRecommendationMenu(Recommender& rec) {
    while (true) {
        subMenu(MovieConstants::TITLE_REC, MovieConstants::OPT_REC);
        int subChoice = readChoice();
        if (subChoice == MovieConstants::MENU_BACK) {
            break;
        }
        switch (subChoice) {
            case MovieConstants::REC_SIM:
                RecommendationHandler::similarUsers(rec);
                break;
            case MovieConstants::REC_USER:
                RecommendationHandler::recommendByUser(rec);
                break;
            case MovieConstants::REC_GENRE:
                RecommendationHandler::recommendByGenre(rec);
                break;
            default:
                cout << "잘못된 선택입니다.\n";
                break;
        }
    }
}

void Menu::handleStatisticsMenu(Statistics& stats) {
    while (true) {
        subMenu(MovieConstants::TITLE_STAT, MovieConstants::OPT_STAT);
        int subChoice = readChoice();
        if (subChoice == MovieConstants::MENU_BACK) {
            break;
        }
        switch (subChoice) {
            case MovieConstants::STAT_ALL:
                StatisticsHandler::overall(stats);
                break;
            case MovieConstants::STAT_TOP_RATE:
                StatisticsHandler::topByRating(stats);
                break;
            case MovieConstants::STAT_TOP_COUNT:
                StatisticsHandler::topByRatingCount(stats);
                break;
            case MovieConstants::STAT_BY_GENRE:
                StatisticsHandler::byGenre(stats);
                break;
            case MovieConstants::STAT_BY_YEAR:
                StatisticsHandler::byYear(stats);
                break;
            case MovieConstants::STAT_TOP_USERS:
                StatisticsHandler::topUsers(stats);
                break;
            case MovieConstants::STAT_USER_DETAILS:
                StatisticsHandler::userStats(stats);
                break;
            default:
                cout << "잘못된 선택입니다.\n";
                break;
        }
    }
}

// Load/Save summaries unchanged
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

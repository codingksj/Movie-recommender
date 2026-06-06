// CLI 메뉴 렌더링, 입력 처리 및 서브메뉴 라우팅

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
using namespace MC::Menu;
using namespace MC::Ui;
using namespace MC::Sort;
using namespace MC::Recommend;
using namespace MC::Score;

static void printMenu(const string& title, const vector<string>& options, const string& exitText) {
    Menu::header(title);
    int numWidth = (int)to_string(options.size()).length();
    for (size_t i = 0; i < options.size(); ++i) {
        cout << "  [" << setw(numWidth) << right << (i + 1) << "] " << options[i] << "\n";
    }
    cout << "  [" << setw(numWidth) << right << 0 << "] " << exitText << "\n";
    cout << "========================================\n";
    cout << "선택> ";
}

static vector<string> formatList(const vector<string>& items) {
    vector<string> lines;
    int numWidth = (int)to_string(items.size()).length();
    for (size_t i = 0; i < items.size(); ++i) {
        stringstream ss;
        ss << "  " << setw(numWidth) << right << (i + 1) << ". " << items[i];
        lines.push_back(ss.str());
    }
    return lines;
}

void Menu::header(const string& title) {
    cout << "\n========================================\n";
    cout << "       " << title << "\n";
    cout << "========================================\n";
}

void Menu::mainMenu() {
    printMenu(Main::TITLE, Main::OPTIONS, "종료 및 저장");
}

void Menu::subMenu(const string& title, const vector<string>& options) {
    printMenu(title, options, "이전 메뉴로");
}

void Menu::resultBox(const string& title, const vector<string>& lines) {
    size_t max_len = title.length() + TITLE_PAD;
    for (const auto& line : lines) {
        if (line.length() > max_len) {
            max_len = line.length();
        }
    }
    if (max_len < MIN_WIDTH) {
        max_len = MIN_WIDTH;
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

void Menu::MovieOp::add(MovieManager& mm) {
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
    auto res = mm.addMovie(title, year, genre);
    if (res.first == MovieResult::DUPLICATE_MOVIE) {
        cout << "이미 존재하는 영화입니다.\n";
    } else {
        cout << "영화가 추가되었습니다. [제목: " << title << ", 연도: " << year << ", 장르: " << genre << "]\n";
    }
    cout << "addMovie took " << fixed << setprecision(3) << res.second << " ms\n";
}

void Menu::MovieOp::search(MovieManager& mm) {
    cout << "검색할 영화 제목: ";
    string query = readLine();
    if (query.empty()) {
        return;
    }
    string outQuery;
    auto res = mm.searchByTitleFormatted(query, outQuery);
    if (res.first.empty()) {
        cout << "[" << outQuery << "] 제목의 영화를 찾을 수 없습니다.\n";
    } else {
        resultBox("영화 검색 결과 (" + outQuery + ")", res.first);
    }
    cout << "search core took " << fixed << setprecision(3) << res.second << " ms\n";
}

void Menu::MovieOp::list(MovieManager& mm) {
    if (mm.getMovies().empty()) {
        cout << "등록된 영화가 없습니다.\n";
        return;
    }
    subMenu("정렬 기준 선택", OPTIONS);
    int sortChoice = readChoice();
    if (sortChoice <= 0 || sortChoice > MAX_OPTION) {
        sortChoice = DEFAULT_CHOICE; 
    }
    string sortName;
    auto res = mm.getSortedMoviesFormatted(sortChoice, sortName);
    resultBox("전체 영화 목록 (정렬: " + sortName + ")", res.first);
    cout << "printAllMovies took " << fixed << setprecision(3) << res.second << " ms\n";
}

void Menu::UserOp::add(UserManager& um) {
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
    auto res = um.addUser(name, email);
    if (res.first == UserResult::DUPLICATE_USER) {
        cout << "[" << name << ", " << email << "]은(는) 이미 존재하는 사용자입니다.\n";
    } else {
        cout << "사용자가 추가되었습니다. [이름: " << name << ", 이메일: " << email << "]\n";
    }
    cout << "addUser took " << fixed << setprecision(3) << res.second << " ms\n";
}

void Menu::UserOp::list(UserManager& um) {
    if (um.getUsers().empty()) {
        cout << "등록된 사용자가 없습니다.\n";
        return;
    }
    vector<string> lines = um.getAllUsersFormatted();
    resultBox("전체 사용자 목록", lines);
}

void Menu::RatingOp::add(RatingManager& rm, MovieManager& mm, UserManager& um) {
    cout << "사용자 이름: ";
    string user = readLine();
    if (user.empty()) {
        return;
    }
    cout << "영화 제목: ";
    string title = readLine();
    if (title.empty()) {
        return;
    }
    cout << "평점 (" << MIN << " ~ " << MAX << "): ";
    double score = readDouble();
    if (std::isnan(score)) {
        cout << "올바른 평점을 입력해주세요.\n";
        return;
    }
    auto res = rm.addRating(user, title, score, mm, um);
    switch (res.first) {
        case RatingResult::USER_NOT_FOUND:
            cout << "[" << user << "]은(는) 등록되지 않은 사용자입니다.\n";
            break;
        case RatingResult::MOVIE_NOT_FOUND:
            cout << "[" << title << "] 제목의 영화를 찾을 수 없습니다.\n";
            break;
        case RatingResult::INVALID_SCORE:
            cout << "올바른 범위의 평점을 입력해주세요.\n";
            break;
        case RatingResult::DUPLICATE_RATING:
            cout << "이미 등록된 평점입니다. [사용자: " << user << ", 영화: " << title << "]\n";
            break;
        default:
            cout << "평점이 등록되었습니다. [사용자: " << user << ", 영화: " << title << ", 평점: " << score << "]\n";
            break;
    }
    cout << "addRating took " << fixed << setprecision(3) << res.second << " ms\n";
}

void Menu::RatingOp::list(RatingManager& rm) {
    cout << "평점을 볼 영화 제목: ";
    string title = readLine();
    vector<string> lines = rm.getRatingsByMovieFormatted(title);
    if (lines.empty()) {
        cout << "등록된 평점이 없습니다.\n";
    } else {
        resultBox("[" + title + "] 평점 목록", lines);
    }
}

void Menu::RecOp::simUsers(Recommender& rec) {
    cout << "유사 사용자를 찾을 사용자 이름 입력: ";
    string user = readLine();
    if (!rec.getUserManager().findUserByName(user)) {
        cout << "존재하지 않는 사용자입니다.\n";
        return;
    }
    if (rec.getUserRatings(user).empty()) {
        cout << "해당 사용자의 평점 이력이 없어 유사도를 계산할 수 없습니다.\n";
        return;
    }
    auto sims = rec.getSimilarUsers(user, SIM_LIMIT);
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
    resultBox(user + "와 유사한 사용자 상위 " + to_string(SIM_LIMIT) + "명", lines);
}

void Menu::RecOp::byUser(Recommender& rec) {
    cout << "추천을 받을 사용자 이름 입력: ";
    string user = readLine();
    if (!rec.getUserManager().findUserByName(user)) {
        cout << "존재하지 않는 사용자입니다.\n";
        return;
    }
    vector<string> items = rec.recommend(user, COLLAB_K, COLLAB_N);
    if (items.empty()) {
        cout << "추천할 만한 새로운 영화가 없습니다.\n";
        return;
    }
    resultBox(user + "님을 위한 추천 영화", formatList(items));
}

void Menu::RecOp::byGenre(Recommender& rec) {
    cout << "기준 영화 제목 입력: ";
    string title = readLine();
    vector<string> items = rec.recommendByGenre(title, GENRE_N);
    if (items.empty()) {
        cout << "추천할 비슷한 장르의 영화가 없거나 존재하지 않는 영화입니다.\n";
        return;
    }
    resultBox(title + "와(과) 비슷한 장르 추천 영화", formatList(items));
}

void Menu::StatOp::overall(Statistics& stats) {
    auto lines = stats.getOverallStatistics();
    resultBox("전체 통계", lines);
}

void Menu::StatOp::topRate(Statistics& stats) {
    auto top = stats.getTopMoviesByRating(TOP_N);
    resultBox("평점 높은 순서 (상위 " + std::to_string(TOP_N) + ")", formatList(top));
}

void Menu::StatOp::topCount(Statistics& stats) {
    auto top = stats.getTopMoviesByRatingCount(TOP_N);
    resultBox("평가 많은 순서 (상위 " + std::to_string(TOP_N) + ")", formatList(top));
}

void Menu::StatOp::byGenre(Statistics& stats) {
    auto genre = stats.getStatisticsByGenre();
    resultBox("장르별 평균 평점", genre);
}

void Menu::StatOp::byYear(Statistics& stats) {
    auto year = stats.getStatisticsByYear();
    resultBox("연도별 평균 평점", year);
}

void Menu::StatOp::topUsers(Statistics& stats) {
    auto top = stats.getTopUsersByRatingCount(TOP_N);
    resultBox("사용자별 평가 수 (상위 " + std::to_string(TOP_N) + ")", formatList(top));
}

void Menu::StatOp::userDetail(Statistics& stats) {
    cout << "사용자 이름 입력: ";
    string user = readLine();
    auto lines = stats.getUserStatistics(user);
    resultBox(user + "님의 통계", lines);
}

void Menu::runMovie(MovieManager& mm) {
    while (true) {
        subMenu(Movie::TITLE, Movie::OPTIONS);
        int subChoice = readChoice();
        if (subChoice == BACK) {
            break;
        }
        switch (subChoice) {
            case Movie::ADD:
                MovieOp::add(mm);
                break;
            case Movie::SEARCH:
                MovieOp::search(mm);
                break;
            case Movie::LIST:
                MovieOp::list(mm);
                break;
            default:
                cout << "잘못된 선택입니다.\n";
                break;
        }
    }
}

void Menu::runUser(UserManager& um) {
    while (true) {
        subMenu(User::TITLE, User::OPTIONS);
        int subChoice = readChoice();
        if (subChoice == BACK) {
            break;
        }
        switch (subChoice) {
            case User::ADD:
                UserOp::add(um);
                break;
            case User::LIST:
                UserOp::list(um);
                break;
            default:
                cout << "잘못된 선택입니다.\n";
                break;
        }
    }
}

void Menu::runRating(RatingManager& rm, MovieManager& mm, UserManager& um) {
    while (true) {
        subMenu(Rating::TITLE, Rating::OPTIONS);
        int subChoice = readChoice();
        if (subChoice == BACK) {
            break;
        }
        switch (subChoice) {
            case Rating::ADD:
                RatingOp::add(rm, mm, um);
                break;
            case Rating::LIST:
                RatingOp::list(rm);
                break;
            default:
                cout << "잘못된 선택입니다.\n";
                break;
        }
    }
}

void Menu::runRec(Recommender& rec) {
    while (true) {
        subMenu(Rec::TITLE, Rec::OPTIONS);
        int subChoice = readChoice();
        if (subChoice == BACK) {
            break;
        }
        switch (subChoice) {
            case Rec::SIM:
                RecOp::simUsers(rec);
                break;
            case Rec::USER:
                RecOp::byUser(rec);
                break;
            case Rec::GENRE:
                RecOp::byGenre(rec);
                break;
            default:
                cout << "잘못된 선택입니다.\n";
                break;
        }
    }
}

void Menu::runStat(Statistics& stats) {
    while (true) {
        subMenu(Stat::TITLE, Stat::OPTIONS);
        int subChoice = readChoice();
        if (subChoice == BACK) {
            break;
        }
        switch (subChoice) {
            case Stat::ALL:
                StatOp::overall(stats);
                break;
            case Stat::TOP_RATE:
                StatOp::topRate(stats);
                break;
            case Stat::TOP_COUNT:
                StatOp::topCount(stats);
                break;
            case Stat::BY_GENRE:
                StatOp::byGenre(stats);
                break;
            case Stat::BY_YEAR:
                StatOp::byYear(stats);
                break;
            case Stat::TOP_USERS:
                StatOp::topUsers(stats);
                break;
            case Stat::USER_DETAIL:
                StatOp::userDetail(stats);
                break;
            default:
                cout << "잘못된 선택입니다.\n";
                break;
        }
    }
}

void Menu::showLoadSummary(int movies, int users, int ratings) {
    cout << "\n========================================\n";
    cout << "          데이터 로드 완료\n";
    cout << "========================================\n";
    cout << "  영화 데이터     : " << movies << "건\n";
    cout << "  사용자 데이터   : " << users << "건\n";
    cout << "  평점 데이터     : " << ratings << "건\n";
    cout << "========================================\n";
}

void Menu::showSaveSummary(int movies, int users, int ratings) {
    cout << "\n========================================\n";
    cout << "       데이터 저장 및 프로그램 종료\n";
    cout << "========================================\n";
    cout << "  영화 데이터     : " << movies << "건 저장 완료\n";
    cout << "  사용자 데이터   : " << users << "건 저장 완료\n";
    cout << "  평점 데이터     : " << ratings << "건 저장 완료\n";
    cout << "========================================\n";
}

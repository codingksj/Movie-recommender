// 프로그램 진입점 — 데이터 초기화 및 메인 메뉴 루프

#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include "Recommender.h"
#include "Statistics.h"
#include "Menu.h"
#include "DataGenerator.h"
#include "MovieConstant.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using std::cout;
using namespace MC::Dataset;
using namespace MC::Menu;

static void createData(size_t movies, size_t users, size_t ratings);
static void promptCounts(size_t& movies, size_t& users, size_t& ratings);
static void initManagers(MovieManager& movieManager, UserManager& userManager, RatingManager& ratingManager);
static void runLoop(MovieManager& movieManager, UserManager& userManager, RatingManager& ratingManager,
                    Recommender& recommender, Statistics& statistics);

int main() {
    MovieManager movieManager;
    UserManager userManager;
    RatingManager ratingManager;
    Recommender recommender(movieManager, ratingManager, userManager);
    Statistics statistics(movieManager, ratingManager, userManager);

    cout << std::fixed << std::setprecision(1);

    initManagers(movieManager, userManager, ratingManager);
    runLoop(movieManager, userManager, ratingManager, recommender, statistics);

    return 0;
}

static size_t readCount(const string& prompt, size_t defaultValue, size_t maxValue) {
    while (true) {
        cout << prompt;
        string input = Menu::readLine();
        if (input.empty()) {
            return defaultValue;
        }

        std::stringstream ss(input);
        int value;
        if (ss >> value && value > 0 && value <= maxValue) {
            return static_cast<size_t>(value);
        }

        cout << "올바른 숫자를 입력해주세요. (1 ~ " << maxValue << ", Enter 입력 시 기본값 사용)\n";
    }
}

static void promptCounts(size_t& movies, size_t& users, size_t& ratings) {
    cout << "[초기 데이터 생성] 생성할 개수를 입력하세요. 엔터를 누르면 기본값을 사용합니다.\n";
    movies = readCount("영화 개수 (기본 " + std::to_string(DEFAULT_MOVIES) + "): ", DEFAULT_MOVIES, MAX_MOVIE_COUNT);
    users = readCount("사용자 개수 (기본 " + std::to_string(DEFAULT_USERS) + "): ", DEFAULT_USERS, MAX_USER_COUNT);
    ratings = readCount("평점 개수 (기본 " + std::to_string(DEFAULT_RATINGS) + "): ", DEFAULT_RATINGS, MAX_RATING_COUNT);

    if (ratings > movies * users) {
        cout << "평점 수는 영화 수 * 사용자 수보다 클 수 없습니다. 기본값(" << DEFAULT_RATINGS << ")로 설정합니다.\n";
        ratings = DEFAULT_RATINGS;
    }
}

static void createData(size_t movies, size_t users, size_t ratings) {
    cout << "[데이터 생성 시작] 합성 영화/사용자/평점 CSV 생성\n";
    bool ok = DataGenerator::generateSyntheticDataset("data/movies.csv",
                                                     "data/users.csv",
                                                     "data/ratings.csv",
                                                     movies,
                                                     users,
                                                     ratings);
    cout << "  데이터 생성: " << (ok ? "성공" : "실패") << "\n";
    cout << "[데이터 생성 완료]\n";
}

static void initManagers(MovieManager& movieManager, UserManager& userManager, RatingManager& ratingManager) {
    size_t movies = DEFAULT_MOVIES;
    size_t users = DEFAULT_USERS;
    size_t ratings = DEFAULT_RATINGS;
    promptCounts(movies, users, ratings);
    createData(movies, users, ratings);
    
    movieManager.setFilePath("data/movies.csv");
    userManager.setFilePath("data/users.csv");
    ratingManager.setFilePath("data/ratings.csv");
    
    movieManager.loadFromFile();
    userManager.loadFromFile();
    ratingManager.loadFromFile();
    ratingManager.mergeRatingsToMovies(movieManager);
    Menu::showLoadSummary(movieManager.size(), userManager.size(), ratingManager.size());
}

// 메인 메뉴 루프 — 사용자 입력 처리 및 기능 라우팅
static void runLoop(MovieManager& movieManager, UserManager& userManager, RatingManager& ratingManager,
                    Recommender& recommender, Statistics& statistics) {
    bool running = true;
    while (running) {
        Menu::mainMenu();
        int mainChoice = Menu::readChoice();
        
        switch (mainChoice) {
            case Main::EXIT: {
                int movies = movieManager.size();
                int users = userManager.size();
                int ratings = ratingManager.size();
                movieManager.saveToFile();
                userManager.saveToFile();
                ratingManager.saveToFile();
                Menu::showSaveSummary(movies, users, ratings);
                running = false;
                break;
            }
            case Main::MOVIE: {
                Menu::runMovie(movieManager);
                break;
            }
            case Main::USER: {
                Menu::runUser(userManager);
                break;
            }
            case Main::RATING: {
                Menu::runRating(ratingManager, movieManager, userManager);
                break;
            }
            case Main::REC: {
                Menu::runRec(recommender);
                break;
            }
            case Main::STAT: {
                Menu::runStat(statistics);
                break;
            }
            default: {
                cout << "잘못된 선택입니다.\n";
                break;
            }
        }
    }
}

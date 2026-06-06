// 메인 진입점: 시스템 초기화, 데이터 로드 및 메뉴 루프

#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include "Recommender.h"
#include "Statistics.h"
#include "Menu.h"
#include "DataGenerator.h"
#include <iostream>
#include <iomanip>

using std::cout;

static void generateDataExtraction(int movieCount, int userCount, int ratingCount) {
    cout << "[데이터 생성 시작] 합성 영화/사용자/평점 CSV 생성\n";
    bool ok = DataGenerator::generateSyntheticDataset("data/movies.csv",
                                                     "data/users.csv",
                                                     "data/ratings.csv",
                                                     movieCount,
                                                     userCount,
                                                     ratingCount);
    cout << "  데이터 생성: " << (ok ? "성공" : "실패") << "\n";
    cout << "[데이터 생성 완료]\n";
}

int main() {
    MovieManager movieManager;
    UserManager userManager;
    RatingManager ratingManager;
    Recommender recommender(movieManager, ratingManager, userManager);
    Statistics statistics(movieManager, ratingManager, userManager);

    cout << std::fixed << std::setprecision(1);

    // (선택) 기본 합성 데이터 생성
    generateDataExtraction(MovieConstants::DEFAULT_MOVIES,
                           MovieConstants::DEFAULT_USERS,
                           MovieConstants::DEFAULT_RATINGS);

    // 데이터 파일 경로 설정 및 로드
    movieManager.setFilePath("data/movies.csv");
    userManager.setFilePath("data/users.csv");
    ratingManager.setFilePath("data/ratings.csv");

    movieManager.loadFromFile();
    movieManager.loadFromFile();
    userManager.loadFromFile();
    ratingManager.loadFromFile();
    ratingManager.mergeRatingsToMovies(movieManager);
    // 로드 완료 요약 출력
    // 로드 완료 안내 배너 출력
    Menu::showLoadSummary(movieManager.size(), userManager.size(), ratingManager.size());

    bool running = true;
    while (running) {
        Menu::mainMenu();
        int mainChoice = Menu::readChoice();

        switch (mainChoice) {
            case MovieConstants::MAIN_EXIT: {
                int movieCount = movieManager.size();
                int userCount = userManager.size();
                int ratingCount = ratingManager.size();
                movieManager.saveToFile();
                userManager.saveToFile();
                ratingManager.saveToFile();
                Menu::showSaveSummary(movieCount, userCount, ratingCount);
                running = false;
                break;
            }
            case MovieConstants::MAIN_MOVIE:
                Menu::handleMovieMenu(movieManager);
                break;
            case MovieConstants::MAIN_USER:
                Menu::handleUserMenu(userManager);
                break;
            case MovieConstants::MAIN_RATING:
                Menu::handleRatingMenu(ratingManager, movieManager, userManager);
                break;
            case MovieConstants::MAIN_REC:
                Menu::handleRecommendationMenu(recommender);
                break;
            case MovieConstants::MAIN_STAT:
                Menu::handleStatisticsMenu(statistics);
                break;
            default:
                cout << "잘못된 선택입니다.\n";
                break;
        }
    }

    return 0;
}
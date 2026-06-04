// 프로그램 진입점 및 메인 제어 루프

#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include "Recommender.h"
#include "Statistics.h"
#include "Menu.h"
#include <iostream>
#include <iomanip>

using std::cout;

// 프로그램의 메인 함수
int main() {
    MovieManager movieManager;
    UserManager userManager;
    RatingManager ratingManager;
    Recommender recommender(movieManager, ratingManager, userManager);
    Statistics statistics(movieManager, ratingManager, userManager);

    cout << std::fixed << std::setprecision(1);

    // 데이터 파일 경로 설정
    movieManager.setFilePath("data/movies.csv");
    userManager.setFilePath("data/users.csv");
    ratingManager.setFilePath("data/ratings.csv");

    // 데이터 로드
    movieManager.loadFromFile();
    userManager.loadFromFile();
    ratingManager.loadFromFile();

    // 로드 완료 안내 배너 출력
    Menu::showLoadSummary(movieManager.size(), userManager.size(), ratingManager.size());

    bool running = true;
    while (running) {
        Menu::showMainMenu();
        int mainChoice = Menu::getChoice();

        if (mainChoice == 0) {
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

        switch (mainChoice) {
            case 1:
                Menu::handleMovieMenu(movieManager);
                break;
            case 2:
                Menu::handleUserMenu(userManager);
                break;
            case 3:
                Menu::handleRatingMenu(ratingManager, movieManager, userManager);
                break;
            case 4:
                Menu::handleRecommendationMenu(recommender);
                break;
            case 5:
                Menu::handleStatisticsMenu(statistics);
                break;
            default:
                cout << "잘못된 선택입니다.\n";
                break;
        }
    }

    return 0;
}
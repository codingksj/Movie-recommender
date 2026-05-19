// 프로그램 진입점 및 메인 제어 루프

#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include "Recommender.h"
#include "menu.h"
#include <iostream>
#include <iomanip>

using std::cout;

// 프로그램의 메인 함수
int main() {
    MovieManager movieManager;
    UserManager userManager;
    RatingManager ratingManager;
    Recommender recommender(movieManager, ratingManager, userManager);

    cout << std::fixed << std::setprecision(1);

    // 데이터 파일 경로 설정
    movieManager.setFilePath("data/movies.csv");
    userManager.setFilePath("data/users.csv");
    ratingManager.setFilePath("data/ratings.csv");

    // 데이터 로드
    movieManager.loadFromFile();
    userManager.loadFromFile();
    ratingManager.loadFromFile();

    bool running = true;
    while (running) {
        showMainMenu();
        int mainChoice = getChoice();

        if (mainChoice == 0) {
            cout << "데이터를 저장하고 프로그램을 종료합니다.\n";
            movieManager.saveToFile();
            userManager.saveToFile();
            ratingManager.saveToFile();
            running = false;
            break;
        }

        switch (mainChoice) {
            case 1:
                handleMovieMenu(movieManager);
                break;
            case 2:
                handleUserMenu(userManager);
                break;
            case 3:
                handleRatingMenu(ratingManager, movieManager, userManager);
                break;
            case 4:
                handleRecommendationMenu(recommender);
                break;
            default:
                cout << "잘못된 선택입니다.\n";
                break;
        }
    }

    return 0;
}
// 프로그램 진입점 — 데이터 초기화 및 메인 메뉴 루프

#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include "Recommender.h"
#include "Statistics.h"
#include "Menu.h"
#include "MovieConstant.h"
#include <iostream>
#include <iomanip>

using std::cout;
using namespace MC::Menu;

static void runLoop(MovieManager& movieManager, UserManager& userManager, RatingManager& ratingManager,
                    Recommender& recommender, Statistics& statistics);

int main() {
    MovieManager movieManager;
    UserManager userManager;
    RatingManager ratingManager;
    Recommender recommender(movieManager, ratingManager, userManager);
    Statistics statistics(movieManager, ratingManager, userManager);

    cout << std::fixed << std::setprecision(1);

    if (!Menu::runStartup(movieManager, userManager, ratingManager)) {
        return 0;
    }
    runLoop(movieManager, userManager, ratingManager, recommender, statistics);

    return 0;
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

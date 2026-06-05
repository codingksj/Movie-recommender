// 프로그램 진입점 및 메인 제어 루프

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

static void sampleDataExtraction(int movieCount, int userCount, int ratingCount) {
    cout << "[샘플 데이터 생성 시작] 합성 영화/사용자/평점 CSV 생성\n";
    bool ok = DataGenerator::generateSyntheticDataset("data/sample_movies.csv",
                                                     "data/sample_users.csv",
                                                     "data/sample_ratings.csv",
                                                     movieCount,
                                                     userCount,
                                                     ratingCount);
    cout << "  합성 데이터 생성: " << (ok ? "성공" : "실패") << "\n";
    cout << "[샘플 데이터 생성 완료]\n";
}

// 프로그램의 메인 함수
int main() {
    MovieManager movieManager;
    UserManager userManager;
    RatingManager ratingManager;
    Recommender recommender(movieManager, ratingManager, userManager);
    Statistics statistics(movieManager, ratingManager, userManager);

    cout << std::fixed << std::setprecision(1);

    // 샘플 합성 데이터 생성을 테스트하려면 아래 함수 호출의 숫자를 수정하세요.
    sampleDataExtraction(MovieConstants::DEFAULT_SAMPLE_MOVIES,
                         MovieConstants::DEFAULT_SAMPLE_USERS,
                         MovieConstants::DEFAULT_SAMPLE_RATINGS);

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
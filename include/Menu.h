// 사용자 입출력 및 메뉴 제어를 분리하여 단일 책임 원칙을 준수하기 위함

#pragma once
#include <string>
#include <vector>
#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include "Recommender.h"

class Menu {
public:
    static void showHeader(const std::string& title);
    static void showMainMenu();
    static void showSubMenu(const std::string& title, const std::vector<std::string>& options);
    static void showDynamicResult(const std::string& title, const std::vector<std::string>& lines);
    static int getChoice();

    static void handleMovieMenu(MovieManager& movieManager);
    static void handleUserMenu(UserManager& userManager);
    static void handleRatingMenu(RatingManager& ratingManager, MovieManager& movieManager, UserManager& userManager);
    static void handleRecommendationMenu(Recommender& recommender);

    static void showLoadSummary(int movieCount, int userCount, int ratingCount);
    static void showSaveSummary(int movieCount, int userCount, int ratingCount);

private:
    static void promptAddMovie(MovieManager& movieManager);
    static void promptSearchMovie(MovieManager& movieManager);
    static void promptPrintMovies(MovieManager& movieManager);

    static void promptAddUser(UserManager& userManager);
    static void promptPrintUsers(UserManager& userManager);

    static void promptAddRating(RatingManager& ratingManager, MovieManager& movieManager, UserManager& userManager);
    static void promptPrintRatings(RatingManager& ratingManager);

    static void promptSimilarUsers(Recommender& recommender);
    static void promptRecommendByUser(Recommender& recommender);
    static void promptRecommendByGenre(Recommender& recommender);
};

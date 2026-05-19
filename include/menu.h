// 메뉴를 깔끔하게 하기 위해 추가하였음.

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
};

#pragma once
#include <string>
#include <vector>
#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include "Recommender.h"

void showHeader(const std::string& title);
void showMainMenu();
void showSubMenu(const std::string& title, const std::vector<std::string>& options);
void showDynamicResult(const std::string& title, const std::vector<std::string>& lines);
int getChoice();

void handleMovieMenu(MovieManager& movieManager);
void handleUserMenu(UserManager& userManager);
void handleRatingMenu(RatingManager& ratingManager, MovieManager& movieManager, UserManager& userManager);
void handleRecommendationMenu(Recommender& recommender);

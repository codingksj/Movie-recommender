// 사용자 입출력 및 메뉴 제어를 분리하여 단일 책임 원칙을 준수하기 위함

#pragma once
#include <string>
#include <vector>

class MovieManager;
class UserManager;
class RatingManager;
class Recommender;
class Statistics;

using std::string;
using std::vector;

class Menu {
public:
    static void header(const string& title);
    static void mainMenu();
    static void subMenu(const string& title, const vector<string>& options);
    static void resultBox(const string& title, const vector<string>& lines);
    static string readLine();
    static int readChoice();
    static double readDouble();

    static void handleMovieMenu(MovieManager& movieMgr);
    static void handleUserMenu(UserManager& userMgr);
    static void handleRatingMenu(RatingManager& ratingMgr, MovieManager& movieMgr, UserManager& userMgr);
    static void handleRecommendationMenu(Recommender& rec);
    static void handleStatisticsMenu(Statistics& stats);

    static void showLoadSummary(int movieCount, int userCount, int ratingCount);
    static void showSaveSummary(int movieCount, int userCount, int ratingCount);

private:
    class MovieHandler {
    public:
        static void add(MovieManager& manager);
        static void search(MovieManager& manager);
        static void print(MovieManager& manager);
    };

    class UserHandler {
    public:
        static void add(UserManager& manager);
        static void print(UserManager& manager);
    };

    class RatingHandler {
    public:
        static void add(RatingManager& rMgr, MovieManager& mMgr, UserManager& uMgr);
        static void print(RatingManager& manager);
    };

    class RecommendationHandler {
    public:
        static void similarUsers(Recommender& manager);
        static void recommendByUser(Recommender& manager);
        static void recommendByGenre(Recommender& manager);
    };

    class StatisticsHandler {
    public:
        static void overall(Statistics& manager);
        static void topByRating(Statistics& manager);
        static void topByRatingCount(Statistics& manager);
        static void byGenre(Statistics& manager);
        static void byYear(Statistics& manager);
        static void topUsers(Statistics& manager);
        static void userStats(Statistics& manager);
    };
};

// CLI 메뉴 렌더링 및 사용자 입력 처리

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

    static void runMovie(MovieManager& mm);
    static void runUser(UserManager& um);
    static void runRating(RatingManager& rm, MovieManager& mm, UserManager& um);
    static void runRec(Recommender& rec);
    static void runStat(Statistics& stats);

    static void showLoadSummary(int movies, int users, int ratings);
    static void showSaveSummary(int movies, int users, int ratings);

private:
    class MovieOp {
    public:
        static void add(MovieManager& mm);
        static void search(MovieManager& mm);
        static void list(MovieManager& mm);
    };

    class UserOp {
    public:
        static void add(UserManager& um);
        static void list(UserManager& um);
    };

    class RatingOp {
    public:
        static void add(RatingManager& rm, MovieManager& mm, UserManager& um);
        static void list(RatingManager& rm);
    };

    class RecOp {
    public:
        static void simUsers(Recommender& rec);
        static void byUser(Recommender& rec);
        static void byGenre(Recommender& rec);
    };

    class StatOp {
    public:
        static void overall(Statistics& stats);
        static void topRate(Statistics& stats);
        static void topCount(Statistics& stats);
        static void byGenre(Statistics& stats);
        static void byYear(Statistics& stats);
        static void topUsers(Statistics& stats);
        static void userDetail(Statistics& stats);
    };
};

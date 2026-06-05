#pragma once

#include <cstddef>
#include <string>
#include <vector>

using std::size_t;
using std::string;
using std::vector;

namespace MovieConstants {

    // 평점 시스템 제약 조건
    constexpr double MIN_RATE = 0.0;
    constexpr double MAX_RATE = 10.0;

    // 입력 스트림 오류 복구 버퍼 크기
    constexpr int IGNORE_LIMIT = 10000;

    // 추천 알고리즘 설정
    constexpr int SIM_LIMIT = 3;
    constexpr int COLLAB_K = 3;
    constexpr int COLLAB_N = 5;
    constexpr int GENRE_N = 5;

    // Synthetic dataset generator defaults and limits
    constexpr size_t MAX_DATASET_COUNT = 1000000; // maximum allowed rows for generated files
    constexpr int DATA_MIN_YEAR = 1888; // earliest valid movie year
    constexpr int DATA_MAX_YEAR = 2026; // latest valid movie year
    constexpr int DEFAULT_SAMPLE_MOVIES = 100;
    constexpr int DEFAULT_SAMPLE_USERS = 50;
    constexpr int DEFAULT_SAMPLE_RATINGS = 200;

    // 메뉴 출력 포맷팅 설정
    constexpr size_t MIN_WIDTH = 40;
    constexpr size_t TITLE_PAD = 6;
    constexpr size_t MAX_STR_LEN = 50;

// New constants for menu behavior
    constexpr int TOP_N = 10; // number of top entries for statistics
    constexpr int DEFAULT_SORT_CHOICE = 3; // default sort index for movie list
    constexpr int MAX_SORT_OPTION = 5; // maximum sort option index

    constexpr int SORT_RATE = 1;
    constexpr int SORT_CNT = 2;
    constexpr int SORT_TITLE = 3;
    constexpr int SORT_GENRE = 4;
    constexpr int SORT_YEAR = 5;

        // Menu option constants
    // Main menu constants
    constexpr int MAIN_EXIT = 0;
    constexpr int MAIN_MOVIE = 1;
    constexpr int MAIN_USER = 2;
    constexpr int MAIN_RATING = 3;
    constexpr int MAIN_REC = 4;
    constexpr int MAIN_STAT = 5;
    constexpr int MENU_BACK = 0;

    enum MovieMenu {
        MOV_ADD = 1,
        MOV_SEARCH,
        MOV_LIST
    };

    enum UserMenu {
        USR_ADD = 1,
        USR_LIST
    };

    enum RatingMenu {
        RAT_ADD = 1,
        RAT_LIST
    };

    enum RecMenu {
        REC_SIM = 1,
        REC_USER,
        REC_GENRE
    };

    enum StatMenu {
        STAT_ALL = 1,
        STAT_TOP_RATE,
        STAT_TOP_COUNT,
        STAT_BY_GENRE,
        STAT_BY_YEAR,
        STAT_TOP_USERS,
        STAT_USER_DETAILS
    };

    inline const string TITLE_MAIN = "영화 추천 시스템";
    inline const string TITLE_MOVIE = "영화 관리";
    inline const string TITLE_USER = "사용자 관리";
    inline const string TITLE_RATING = "평점 관리";
    inline const string TITLE_REC = "추천 서비스";
    inline const string TITLE_STAT = "통계 서비스";

    // 메뉴 선택 옵션 리스트
    inline const vector<string> OPT_MAIN = {
        "영화 관리", "사용자 관리", "평점 관리", "추천 서비스", "통계 서비스"
    };
    inline const vector<string> OPT_MOVIE = {
        "Add movie", "Search title", "All movies"
    };
    inline const vector<string> OPT_USER = {
        "Add user", "User list"
    };
    inline const vector<string> OPT_RATING = {
        "Add rating", "View ratings"
    };
    inline const vector<string> OPT_REC = {
        "Similar users", "Recommend by user", "Recommend by genre"
    };
    inline const vector<string> OPT_STAT = {
        "전체 통계", "평점 순위 (상위 10)", "평가 수 순위 (상위 10)",
        "장르별 통계", "연도별 통계", "사용자별 평가 순위", "사용자 상세 정보"
    };
    inline const vector<string> OPT_SORT = {
        "Top rating", "Most reviews", "Title", "Genre", "Year (recent)"
    };

} // namespace MovieConstants

enum class MovieResult {
    SUCCESS,
    DUPLICATE_MOVIE
};

enum class UserResult {
    SUCCESS,
    DUPLICATE_USER
};

enum class RatingResult {
    SUCCESS,
    USER_NOT_FOUND,
    MOVIE_NOT_FOUND,
    INVALID_SCORE,
    DUPLICATE_RATING
};

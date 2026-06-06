#pragma once

#include <cstddef>
#include <string>
#include <vector>

using std::size_t;
using std::string;
using std::vector;

namespace MovieConstants {

    // 평점 범위: 입력 검증과 통계 일관성 확보를 위한 최소/최대 값
    constexpr double MIN_RATE = 0.0;
    constexpr double MAX_RATE = 10.0;

    // 입력 처리 안정성 한계 (읽기 무시 최대치)
    constexpr int IGNORE_LIMIT = 10'000;

    // 추천 알고리즘 기본값(성능/정확도 균형)
    constexpr int SIM_LIMIT = 3;
    constexpr int COLLAB_K = 3;
    constexpr int COLLAB_N = 5;
    constexpr int GENRE_N = 5;

    // 합성 데이터 생성기 기본값 및 상한 (성능/현실성 균형)
    constexpr size_t MAX_MOVIE_COUNT = 100'000;
    constexpr size_t MAX_USER_COUNT = 1'000'000;
    constexpr size_t MAX_RATING_COUNT = 1'000'000;
    constexpr size_t MAX_DATASET_COUNT = 1'000'000;
    constexpr int DATA_MIN_YEAR = 1888;
    constexpr int DATA_MAX_YEAR = 2026;
    constexpr int DEFAULT_MOVIES = 100;
    constexpr int DEFAULT_USERS = 50;
    constexpr int DEFAULT_RATINGS = 200;

    // 콘솔 메뉴 출력 기본 형식
    constexpr size_t MIN_WIDTH = 40;
    constexpr size_t TITLE_PAD = 6;
    constexpr size_t MAX_STR_LEN = 50;

    // 메뉴 동작 기본값
    constexpr int TOP_N = 10;
    constexpr int DEFAULT_SORT_CHOICE = 3;
    constexpr int MAX_SORT_OPTION = 5;

    constexpr int SORT_RATE = 1;
    constexpr int SORT_CNT = 2;
    constexpr int SORT_TITLE = 3;
    constexpr int SORT_GENRE = 4;
    constexpr int SORT_YEAR = 5;

    // 메인 메뉴 식별자
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

    // 메뉴 라벨 모음
    inline const vector<string> OPT_MAIN = {
        "영화 관리", "사용자 관리", "평점 관리", "추천 서비스", "통계 서비스"
    };
    inline const vector<string> OPT_MOVIE = {
        "영화 추가", "제목 검색", "영화 조회(정렬 포함)"
    };
    inline const vector<string> OPT_USER = {
        "사용자 추가", "사용자 목록"
    };
    inline const vector<string> OPT_RATING = {
        "평점 추가", "평점 보기"
    };
    inline const vector<string> OPT_REC = {
        "유사 사용자", "사용자별 추천", "장르별 추천"
    };
    inline const vector<string> OPT_STAT = {
        "전체 통계", "평점 순위 (상위 10)", "평가 수 순위 (상위 10)",
        "장르별 통계", "연도별 통계", "사용자별 평가 순위", "사용자 상세 정보"
    };
    inline const vector<string> OPT_SORT = {
        "최고 평점 순", "평가 많은 순", "제목 사전순", "장르순", "연도순 (최신)"
    };

    // 데이터 생성기 상수 (재현성 및 포맷 고정)
    namespace Generator {
        constexpr int SCORE_TENTHS_MIN = 0;
        constexpr int SCORE_TENTHS_MAX = 100;
        constexpr double Z_MEAN = 0.0;
        constexpr double Z_STDDEV = 1.0;
        constexpr double SCORE_MAX = 10.0;
        constexpr double SCORE_SCALE = 10.0;
        constexpr double CDF_HALF = 0.5;
        constexpr double SQRT2 = 1.4142135623730951;
        constexpr int MIN_RATING = 1;

        constexpr int DIGIT_MIN = 10;
        constexpr int DIGIT_MAX = 99;

        constexpr int SERIES_LENGTH = 7;
        inline const vector<string> ROMANS = {"I","II","III","IV","V","VI","VII"};

        constexpr int PCT_SCALE = 100;
        constexpr int PCT_ROUND = 50;
    }

} 

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

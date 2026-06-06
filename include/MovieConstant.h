#pragma once

#include <cstddef>
#include <string>
#include <vector>

using std::size_t;
using std::string;
using std::vector;

// 프로젝트 전역 상수 — 도메인별 중첩 네임스페이스로 분류
// 호출부에서는 하단 MC 별칭과 파일 단위 using 선언으로 경로를 줄여 사용
namespace MovieConstants {

namespace Score {
    constexpr double MIN = 0.0;
    constexpr double MAX = 10.0;
}

namespace Input {
    constexpr int IGNORE_LIMIT = 10'000;
    constexpr size_t MAX_STR_LEN = 50;
}

namespace Recommend {
    constexpr int SIM_LIMIT = 3;
    constexpr int COLLAB_K = 3;
    constexpr int COLLAB_N = 5;
    constexpr int GENRE_N = 5;
}

namespace Dataset {
    constexpr size_t MAX_MOVIE_COUNT = 100'000;
    constexpr size_t MAX_USER_COUNT = 1'000'000;
    constexpr size_t MAX_RATING_COUNT = 1'000'000;
    constexpr size_t MAX_COUNT = 1'000'000;
    constexpr int MIN_YEAR = 1888;
    constexpr int MAX_YEAR = 2026;
    constexpr int DEFAULT_MOVIES = 100;
    constexpr int DEFAULT_USERS = 50;
    constexpr int DEFAULT_RATINGS = 200;
}

namespace Ui {
    constexpr size_t MIN_WIDTH = 40;
    constexpr size_t TITLE_PAD = 6;
    constexpr int TOP_N = 10;
    
    // 포맷팅 너비 — 데이터 출력 정렬을 위해 명시적으로 선언
    // 영화/사용자/평점 통계 출력 시 칼럼 정렬 유지 필요
    constexpr int FORMAT_TITLE_WIDTH = 30;           // 영화 제목 너비
    constexpr int FORMAT_USER_WIDTH = 20;            // 사용자명 너비
    constexpr int FORMAT_GENRE_WIDTH = 10;           // 장르 너비
    constexpr int FORMAT_RATING_COUNT_WIDTH = 7;     // 평가 수 너비
    constexpr int FORMAT_SCORE_WIDTH = 4;            // 평점 너비
    constexpr int FORMAT_COUNT_WIDTH = 3;            // 개수/갯수 너비
    constexpr int FORMAT_AVG_WIDTH = 5;              // 평균값 너비
    constexpr int FORMAT_YEAR_WIDTH = 4;             // 연도 너비
}

namespace Sort {
    constexpr int DEFAULT_CHOICE = 3;
    constexpr int MAX_OPTION = 5;
    constexpr int RATE = 1;
    constexpr int CNT = 2;
    constexpr int TITLE = 3;
    constexpr int GENRE = 4;
    constexpr int YEAR = 5;

    inline const vector<string> OPTIONS = {
        "최고 평점 순", "평가 많은 순", "제목 사전순", "장르순", "연도순 (최신)"
    };
}

namespace Menu {
    constexpr int BACK = 0;

    namespace Main {
        constexpr int EXIT = 0;
        constexpr int MOVIE = 1;
        constexpr int USER = 2;
        constexpr int RATING = 3;
        constexpr int REC = 4;
        constexpr int STAT = 5;

        inline const string TITLE = "영화 추천 시스템";
        inline const vector<string> OPTIONS = {
            "영화 관리", "사용자 관리", "평점 관리", "추천 서비스", "통계 서비스"
        };
    }

    namespace Movie {
        constexpr int ADD = 1;
        constexpr int SEARCH = 2;
        constexpr int LIST = 3;

        inline const string TITLE = "영화 관리";
        inline const vector<string> OPTIONS = {
            "영화 추가", "제목 검색", "영화 조회(정렬 포함)"
        };
    }

    namespace User {
        constexpr int ADD = 1;
        constexpr int LIST = 2;

        inline const string TITLE = "사용자 관리";
        inline const vector<string> OPTIONS = {
            "사용자 추가", "사용자 목록"
        };
    }

    namespace Rating {
        constexpr int ADD = 1;
        constexpr int LIST = 2;

        inline const string TITLE = "평점 관리";
        inline const vector<string> OPTIONS = {
            "평점 추가", "평점 보기"
        };
    }

    namespace Rec {
        constexpr int SIM = 1;
        constexpr int USER = 2;
        constexpr int GENRE = 3;

        inline const string TITLE = "추천 서비스";
        inline const vector<string> OPTIONS = {
            "유사 사용자", "사용자별 추천", "장르별 추천"
        };
    }

    namespace Stat {
        constexpr int ALL = 1;
        constexpr int TOP_RATE = 2;
        constexpr int TOP_COUNT = 3;
        constexpr int BY_GENRE = 4;
        constexpr int BY_YEAR = 5;
        constexpr int TOP_USERS = 6;
        constexpr int USER_DETAIL = 7;

        inline const string TITLE = "통계 서비스";
        inline const vector<string> OPTIONS = {
            "전체 통계", "평점 순위 (상위 10)", "평가 수 순위 (상위 10)",
            "장르별 통계", "연도별 통계", "사용자별 평가 순위", "사용자 상세 정보"
        };
    }
}

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
    constexpr int PCT_SCALE = 100;
    constexpr int PCT_ROUND = 50;

    constexpr size_t PATTERN_ADJ_NOUN = 0;
    constexpr size_t PATTERN_ADJ_ADJ_NOUN = 1;
    constexpr size_t PATTERN_THE_NOUN_OF_ADJ_NOUN = 2;
    constexpr size_t PATTERN_NOUN_COLON_ADJ_NOUN = 3;
    constexpr size_t PATTERN_COUNT = 4;

    inline const string SEED_FILE_PATH = "data/generate_seed.csv";
    inline const string SEED_TYPE_BASE = "base";
    inline const string SEED_TYPE_SUFFIX = "suffix";
    inline const string SEED_TYPE_GENRE = "genre";
    inline const string SEED_TYPE_TITLE_ADJ = "title_adj";
    inline const string SEED_TYPE_TITLE_NOUN = "title_noun";

    inline const vector<string> ROMANS = {"I", "II", "III", "IV", "V", "VI", "VII"};
}

} // namespace MovieConstants

namespace MC = MovieConstants;

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

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

    // 메뉴 출력 포맷팅 설정
    constexpr size_t MIN_WIDTH = 40;
    constexpr size_t TITLE_PAD = 6;

    // 정렬 선택 번호 상수
    constexpr int SORT_RATE = 1;
    constexpr int SORT_CNT = 2;
    constexpr int SORT_TITLE = 3;
    constexpr int SORT_GENRE = 4;
    constexpr int SORT_YEAR = 5;

    // 메뉴 타이틀 문자열
    inline const string TITLE_MAIN = "MOVIE RECOMMENDER SYSTEM";
    inline const string TITLE_MOVIE = "영화 관리";
    inline const string TITLE_USER = "사용자 관리";
    inline const string TITLE_RATING = "평점 관리";
    inline const string TITLE_RECOMMEND = "추천 서비스";

    // 메뉴 선택 옵션 리스트
    inline const vector<string> OPTIONS_MAIN = {
        "영화 관리", "사용자 관리", "평점 관리", "추천 서비스"
    };
    inline const vector<string> OPTIONS_MOVIE = {
        "영화 추가", "제목으로 검색", "전체 목록 (정렬 포함)"
    };
    inline const vector<string> OPTIONS_USER = {
        "사용자 추가", "사용자 목록 출력"
    };
    inline const vector<string> OPTIONS_RATING = {
        "평점 입력", "영화별 평점 보기"
    };
    inline const vector<string> OPTIONS_RECOMMEND = {
        "유사 사용자 찾기", "영화 추천 받기 (사용자 기준)", "영화 추천 받기 (장르 유사도 기준)"
    };
    inline const vector<string> OPTIONS_SORT = {
        "평점 높은순", "평점 많은순", "제목 사전순", "장르 사전순", "개봉연도순 (최신순)"
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

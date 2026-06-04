#pragma once

#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;
using std::pair;

// 영화 추천 시스템의 다양한 통계 정보를 계산하고 제공하기 위한 클래스
class Statistics {
private:
    const MovieManager& movieManager;
    const RatingManager& ratingManager;
    const UserManager& userManager;

public:
    Statistics(const MovieManager& mm, const RatingManager& rm, const UserManager& um)
        : movieManager(mm), ratingManager(rm), userManager(um) {}

    // ===== 전체 통계 =====
    // 전체 데이터 요약 (영화수, 사용자수, 평점수, 평균 평점)
    vector<string> getOverallStatistics() const;

    // ===== 영화 통계 =====
    // 평점 기준 Top N 영화 (평점 높은순)
    vector<string> getTopMoviesByRating(int n = 10) const;
    
    // 평가 수 기준 Top N 영화 (평가 많은순)
    vector<string> getTopMoviesByRatingCount(int n = 10) const;
    
    // 장르별 평균 평점 및 영화 수
    vector<string> getStatisticsByGenre() const;
    
    // 개봉연도별 평균 평점
    vector<string> getStatisticsByYear() const;

    // ===== 사용자 통계 =====
    // 사용자별 평가 수 및 평균 평점 (많은순)
    vector<string> getTopUsersByRatingCount(int n = 10) const;
    
    // 특정 사용자 상세 통계
    vector<string> getUserStatistics(const string& userName) const;

private:
    // 헬퍼 메서드들
    // 전체 영화의 평균 평점 계산
    double calculateAverageRating() const;
    
    // 장르별 평균 평점 맵 생성
    map<string, pair<double, int>> getGenreStatistics() const;
    
    // 연도별 평균 평점 맵 생성
    map<int, pair<double, int>> getYearStatistics() const;
    
    // 사용자별 평가 통계 맵 생성
    map<string, pair<int, double>> getUserRatingStatistics() const;
};

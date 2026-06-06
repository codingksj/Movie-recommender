#pragma once

#include <string>
#include <vector>
#include <random>

using std::string;
using std::vector;

class DataGenerator {
public:
    static bool generateSyntheticDataset(const string& moviePath,
                                         const string& userPath,
                                         const string& ratingPath,
                                         size_t movieCnt,
                                         size_t userCnt,
                                         size_t ratingCnt,
                                         int okPercent = 0);

private:
    // 헬퍼 함수들 - 거대 함수 분해
    static bool validateGenerationParams(size_t movieCnt, size_t userCnt, 
                                        size_t ratingCnt, int okPercent);
    
    static void generateMovieData(std::mt19937_64& engine,
                                 const vector<string>& titleAdjectives,
                                 const vector<string>& titleNouns,
                                 const vector<string>& genres,
                                 size_t movieCnt,
                                 vector<string>& outMovies,
                                 vector<string>& outGenres,
                                 vector<int>& outYears);
    
    static bool generateUserData(std::mt19937_64& engine,
                                const vector<string>& userBases,
                                const vector<string>& userSuffixes,
                                size_t userCnt,
                                const string& userPath,
                                vector<string>& outUsers);
    
    static void calculateRatingDistribution(size_t movieCnt, size_t userCnt,
                                           size_t ratingCnt,
                                           vector<int>& outFlatDist);
    
    static bool generateAndSaveRatings(std::mt19937_64& engine,
                                      const vector<string>& movies,
                                      const vector<string>& users,
                                      const vector<int>& ratingDist,
                                      size_t movieCnt,
                                      size_t userCnt,
                                      const string& ratingPath,
                                      vector<double>& outMovieTotalRatings,
                                      vector<int>& outMovieRatingCounts);
    
    static bool saveMoviesToFile(const vector<string>& movies,
                                const vector<string>& genres,
                                const vector<int>& years,
                                const vector<double>& totalRatings,
                                const vector<int>& ratingCounts,
                                const string& moviePath);
};

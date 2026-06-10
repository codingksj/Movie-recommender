#pragma once

#include <string>
#include <vector>
#include <random>

using std::string;
using std::vector;

class DataGenerator {
public:
    struct GenConfig {
        string moviePath;
        string userPath;
        string ratingPath;
        size_t movieCnt = 0;
        size_t userCnt = 0;
        size_t ratingCnt = 0;
        int okPercent = 0;
    };

    struct RatingConfig {
        const vector<string>& movies;
        const vector<string>& users;
        const vector<size_t>& perMovieCnt;
        size_t movieCnt = 0;
        size_t userCnt = 0;
        string ratingPath;
    };

    struct RatingOutputs {
        vector<double>& movieTotalRatings;
        vector<int>& movieRatingCounts;
    };

    struct MovieSaveParams {
        const vector<string>& movies;
        const vector<string>& genres;
        const vector<int>& years;
        const vector<double>& totalRatings;
        const vector<int>& ratingCounts;
        string moviePath;
    };

    struct GenResult {
        bool ok = false;
        double ms = 0.0;
    };

    static GenResult generate(const GenConfig& config);

private:
    static vector<string> titleAdj;
    static vector<string> titleNoun;
    static vector<string> genre;
    static vector<string> userBase;
    static vector<string> userSuffix;
    
    static bool loadSeeds(const string& filePath);
    static bool validate(size_t movieCnt, size_t userCnt, 
                         size_t ratingCnt, int okPercent);
    
    static void genMovies(std::mt19937_64& engine,
                          size_t movieCnt,
                          vector<string>& outMovies,
                          vector<string>& outGenres,
                          vector<int>& outYears);
    
    static bool genUsers(std::mt19937_64& engine,
                         size_t userCnt,
                         const string& userPath,
                         vector<string>& outUsers);
    
    static void buildPerMovieCnt(size_t movieCnt, size_t ratingCnt,
                                 vector<size_t>& outPerMovieCnt);

    static bool genRatings(std::mt19937_64& engine,
                           const RatingConfig& config,
                           RatingOutputs& outputs);
    
    static bool saveMovies(const MovieSaveParams& params);
};

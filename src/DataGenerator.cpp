// 합성 영화·사용자·평점 CSV 생성

#include "DataGenerator.h"
#include <fstream>
#include "MovieConstant.h"
#include <random>
#include <set>
#include <vector>
#include <sstream>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <iomanip>

using std::string;
using std::vector;
using namespace MC::Dataset;
using namespace MC::Generator;

static string trimString(const string& value) {
    size_t begin = 0;
    while (begin < value.size() && std::isspace((unsigned char)(value[begin]))) {
        ++begin;
    }
    size_t end = value.size();
    while (end > begin && std::isspace((unsigned char)(value[end - 1]))) {
        --end;
    }
    return value.substr(begin, end - begin);
}

static bool loadSeedPools(const string& filePath,
                          vector<string>& baseWords,
                          vector<string>& suffixWords,
                          vector<string>& genres,
                          vector<string>& titleAdjectives,
                          vector<string>& titleNouns) {
    std::ifstream seedFile(filePath);
    if (!seedFile.is_open()) {
        return false;
    }

    string header;
    getline(seedFile, header);
    string line;
    std::set<string> seen;

    while (getline(seedFile, line)) {
        if (line.empty()) {
            continue;
        }
        string type;
        string value;
        std::stringstream ss(line);
        if (!getline(ss, type, ',')) {
            continue;
        }
        if (!getline(ss, value)) {
            continue;
        }

        type = trimString(type);
        value = trimString(value);
        if (value.empty()) {
            continue;
        }
        string uniqueKey = type + ":" + value;
        if (!seen.insert(uniqueKey).second) {
            continue;
        }

        if (type == SEED_TYPE_BASE) {
            baseWords.push_back(value);
        } else if (type == SEED_TYPE_SUFFIX) {
            suffixWords.push_back(value);
        } else if (type == SEED_TYPE_GENRE) {
            genres.push_back(value);
        } else if (type == SEED_TYPE_TITLE_ADJ) {
            titleAdjectives.push_back(value);
        } else if (type == SEED_TYPE_TITLE_NOUN) {
            titleNouns.push_back(value);
        }
    }

    return !baseWords.empty() && !suffixWords.empty() && !genres.empty()
        && !titleAdjectives.empty() && !titleNouns.empty();
}

static string escapeCsvField(const string& field) {
    if (field.find_first_of(",\"") == string::npos) {
        return field;
    }
    string escaped;
    escaped.push_back('"');
    for (char c : field) {
        if (c == '"') {
            escaped.append("\"");
        } else {
            escaped.push_back(c);
        }
    }
    escaped.push_back('"');
    return escaped;
}

// ============================================================================
// 헬퍼 함수 구현부 — 생성 로직을 작은 단위로 분해
// ============================================================================

// 입력 매개변수 유효성 검사 — 생성 불가능한 조건 사전 차단
bool DataGenerator::validateGenerationParams(size_t movieCnt, size_t userCnt,
                                            size_t ratingCnt, int okPercent) {
    if (movieCnt == 0 || userCnt == 0 || ratingCnt == 0) {
        return false;
    }
    if (movieCnt > MAX_MOVIE_COUNT || userCnt > MAX_USER_COUNT || ratingCnt > MAX_RATING_COUNT) {
        return false;
    }
    if (okPercent < 0 || okPercent > PCT_SCALE) {
        return false;
    }
    if (ratingCnt > movieCnt * userCnt) {
        return false;
    }
    return true;
}

// 영화 데이터 생성 — 제목, 장르, 개봉연도를 조합으로 생성
void DataGenerator::generateMovieData(std::mt19937_64& engine,
                                     const vector<string>& titleAdjectives,
                                     const vector<string>& titleNouns,
                                     const vector<string>& genres,
                                     size_t movieCnt,
                                     vector<string>& outMovies,
                                     vector<string>& outGenres,
                                     vector<int>& outYears) {
    outMovies.reserve(movieCnt);
    outGenres.reserve(movieCnt);
    outYears.reserve(movieCnt);

    std::uniform_int_distribution<size_t> adjDist(0, titleAdjectives.size() - 1);
    std::uniform_int_distribution<size_t> nounDist(0, titleNouns.size() - 1);
    std::uniform_int_distribution<size_t> genreDist(0, genres.size() - 1);
    std::uniform_int_distribution<size_t> patternDist(0, PATTERN_COUNT - 1);
    std::uniform_int_distribution<int> yearDist(MIN_YEAR, MAX_YEAR);

    const auto& romans = ROMANS;
    for (size_t i = 1; i <= movieCnt; ++i) {
        string title;
        const string& adj1 = titleAdjectives[adjDist(engine)];
        const string& adj2 = titleAdjectives[adjDist(engine)];
        const string& noun1 = titleNouns[nounDist(engine)];
        const string& noun2 = titleNouns[nounDist(engine)];
        
        switch (patternDist(engine)) {
            case PATTERN_ADJ_NOUN: {
                title = adj1 + " " + noun1;
                break;
            }
            case PATTERN_ADJ_ADJ_NOUN: {
                title = adj1 + " " + adj2 + " " + noun1;
                break;
            }
            case PATTERN_THE_NOUN_OF_ADJ_NOUN: {
                title = string("The ") + noun1 + " of " + adj1 + " " + noun2;
                break;
            }
            case PATTERN_NOUN_COLON_ADJ_NOUN: {
                title = noun1 + ": " + adj1 + " " + noun2;
                break;
            }
            default: {
                title = adj1 + " " + noun1;
                break;
            }
        }
        
        int series = (int)(((i - 1) % SERIES_LENGTH) + 1);
        if (series > 1 && series <= SERIES_LENGTH) {
            title += " " + romans[series - 1];
        }

        outMovies.push_back(title);
        outGenres.push_back(genres[genreDist(engine)]);
        outYears.push_back(yearDist(engine));
    }
}

// 사용자 데이터 생성 및 저장 — CSV 파일에 사용자 정보 기록
bool DataGenerator::generateUserData(std::mt19937_64& engine,
                                    const vector<string>& userBases,
                                    const vector<string>& userSuffixes,
                                    size_t userCnt,
                                    const string& userPath,
                                    vector<string>& outUsers) {
    outUsers.reserve(userCnt);
    
    std::uniform_int_distribution<size_t> baseDist(0, userBases.size() - 1);
    std::uniform_int_distribution<size_t> suffixDist(0, userSuffixes.size() - 1);
    std::uniform_int_distribution<int> digitDist(DIGIT_MIN, DIGIT_MAX);

    std::set<string> usedUserNames;
    std::ofstream userOut(userPath);
    if (!userOut.is_open()) {
        return false;
    }
    
    userOut << "userName,userEmail\n";
    for (size_t i = 1; i <= userCnt; ++i) {
        string userName;
        do {
            string base = userBases[baseDist(engine)];
            int digits = digitDist(engine);
            string suffix = userSuffixes[suffixDist(engine)];
            std::ostringstream oss;
            oss << base << digits << suffix;
            userName = oss.str();
        } while (!usedUserNames.insert(userName).second);

        string email = userName + "@example.com";
        outUsers.push_back(userName);
        userOut << escapeCsvField(userName) << ',' << escapeCsvField(email) << '\n';
    }
    
    return true;
}

// 평점 분포 계산 — 희소 행렬 패턴으로 평점 위치 결정
void DataGenerator::calculateRatingDistribution(size_t movieCnt, size_t userCnt,
                                               size_t ratingCnt,
                                               vector<int>& outFlatDist) {
    size_t blockSize = (ratingCnt + movieCnt - 1) / movieCnt;
    size_t totalSlots = blockSize * movieCnt;
    outFlatDist.resize(totalSlots, 0);
    
    for (size_t k = 0; k < ratingCnt; ++k) {
        outFlatDist[k] = 1;
    }
    
    std::random_device rd;
    std::mt19937_64 shuffleEngine(rd());
    std::shuffle(outFlatDist.begin(), outFlatDist.end(), shuffleEngine);
}

// 평점 생성 및 저장 — 정규분포 기반 평점 생성 후 CSV 기록
bool DataGenerator::generateAndSaveRatings(std::mt19937_64& engine,
                                          const vector<string>& movies,
                                          const vector<string>& users,
                                          const vector<int>& ratingDist,
                                          size_t movieCnt,
                                          size_t userCnt,
                                          const string& ratingPath,
                                          vector<double>& outMovieTotalRatings,
                                          vector<int>& outMovieRatingCounts) {
    outMovieTotalRatings.assign(movieCnt, 0.0);
    outMovieRatingCounts.assign(movieCnt, 0);

    std::ofstream ratingOut(ratingPath);
    if (!ratingOut.is_open()) {
        return false;
    }
    ratingOut << "userName,movieTitle,userRating\n";

    std::normal_distribution<double> scoreNormDist(Z_MEAN, Z_STDDEV);
    size_t blockSize = (ratingDist.size() + movieCnt - 1) / movieCnt;
    
    vector<size_t> userIndices(userCnt);
    for (size_t i = 0; i < userCnt; ++i) {
        userIndices[i] = i;
    }

    size_t generated = 0;
    for (size_t mi = 0; mi < movieCnt; ++mi) {
        size_t start = mi * blockSize;
        size_t end = start + blockSize;
        size_t movieRatings = 0;
        
        for (size_t s = start; s < end && s < ratingDist.size(); ++s) {
            if (ratingDist[s]) {
                ++movieRatings;
            }
        }
        if (movieRatings == 0) {
            continue;
        }

        std::shuffle(userIndices.begin(), userIndices.end(), engine);
        size_t take = std::min(movieRatings, userCnt);
        
        for (size_t t = 0; t < take; ++t) {
            size_t uidx = userIndices[t];
            double z = scoreNormDist(engine);
            double u = CDF_HALF * (1.0 + std::erf(z / SQRT2));
            double mapped = u * SCORE_MAX;
            int scoreInt = (int)std::round(mapped * SCORE_SCALE);
            
            if (scoreInt < SCORE_TENTHS_MIN) {
                scoreInt = SCORE_TENTHS_MIN;
            }
            if (scoreInt > SCORE_TENTHS_MAX) {
                scoreInt = SCORE_TENTHS_MAX;
            }
            
            double score = scoreInt / SCORE_SCALE;
            ratingOut << escapeCsvField(users[uidx]) << ',' << escapeCsvField(movies[mi]) << ',' << std::fixed << std::setprecision(1) << score << '\n';
            outMovieTotalRatings[mi] += score;
            ++outMovieRatingCounts[mi];
            ++generated;
        }
        
        for (size_t extra = take; extra < movieRatings; ++extra) {
            size_t uidx = engine() % userCnt;
            double z = scoreNormDist(engine);
            double u = CDF_HALF * (1.0 + std::erf(z / SQRT2));
            double mapped = u * SCORE_MAX;
            int scoreInt = (int)std::round(mapped * SCORE_SCALE);
            
            if (scoreInt < SCORE_TENTHS_MIN) {
                scoreInt = SCORE_TENTHS_MIN;
            }
            if (scoreInt > SCORE_TENTHS_MAX) {
                scoreInt = SCORE_TENTHS_MAX;
            }
            
            double score = scoreInt / SCORE_SCALE;
            ratingOut << escapeCsvField(users[uidx]) << ',' << escapeCsvField(movies[mi]) << ',' << std::fixed << std::setprecision(1) << score << '\n';
            outMovieTotalRatings[mi] += score;
            ++outMovieRatingCounts[mi];
            ++generated;
        }
    }
    
    return generated > 0;
}

// 영화 파일 저장 — 생성된 영화 데이터와 평점 통계를 CSV로 기록
bool DataGenerator::saveMoviesToFile(const vector<string>& movies,
                                    const vector<string>& genres,
                                    const vector<int>& years,
                                    const vector<double>& totalRatings,
                                    const vector<int>& ratingCounts,
                                    const string& moviePath) {
    std::ofstream movieOut(moviePath);
    if (!movieOut.is_open()) {
        return false;
    }
    
    movieOut << "title,genre,year,totalRating,ratingCount\n";
    for (size_t mi = 0; mi < movies.size(); ++mi) {
        movieOut << escapeCsvField(movies[mi]) << ',' << escapeCsvField(genres[mi]) << ',' << years[mi] << ','
                 << std::fixed << std::setprecision(1) << totalRatings[mi] << ',' << ratingCounts[mi] << '\n';
    }
    
    return true;
}

// ============================================================================
// generateSyntheticDataset — 메인 함수 (헬퍼 함수 호출 조율)
// ============================================================================

bool DataGenerator::generateSyntheticDataset(const string& moviePath,
                                             const string& userPath,
                                             const string& ratingPath,
                                             size_t movieCnt,
                                             size_t userCnt,
                                             size_t ratingCnt,
                                             int okPercent) {
    // 1. 입력 매개변수 유효성 검사
    if (!validateGenerationParams(movieCnt, userCnt, ratingCnt, okPercent)) {
        return false;
    }

    auto generationStart = std::chrono::high_resolution_clock::now();

    // 2. 시드 데이터 로드
    vector<string> titleAdjectives;
    vector<string> titleNouns;
    vector<string> userBases;
    vector<string> userSuffixes;
    vector<string> genres;
    if (!loadSeedPools(SEED_FILE_PATH, userBases, userSuffixes, genres, titleAdjectives, titleNouns)) {
        std::cerr << "Seed file 로드를 실패했습니다: " << SEED_FILE_PATH << "\n";
        return false;
    }

    // 3. 난수 엔진 초기화
    std::random_device rd;
    std::mt19937_64 engine(rd());

    // 4. 영화 데이터 생성
    vector<string> movies;
    vector<string> movieGenres;
    vector<int> movieYears;
    generateMovieData(engine, titleAdjectives, titleNouns, genres, movieCnt, movies, movieGenres, movieYears);

    // 5. 사용자 데이터 생성
    vector<string> users;
    if (!generateUserData(engine, userBases, userSuffixes, userCnt, userPath, users)) {
        return false;
    }

    // 6. 평점 분포 계산
    vector<int> ratingDist;
    calculateRatingDistribution(movieCnt, userCnt, ratingCnt, ratingDist);

    // 7. 평점 생성 및 저장
    vector<double> movieTotalRatings;
    vector<int> movieRatingCounts;
    size_t generated = 0;
    if (!generateAndSaveRatings(engine, movies, users, ratingDist, movieCnt, userCnt, ratingPath, movieTotalRatings, movieRatingCounts)) {
        return false;
    }
    generated = std::accumulate(movieRatingCounts.begin(), movieRatingCounts.end(), (size_t)0);

    // 8. 영화 파일 저장
    if (!saveMoviesToFile(movies, movieGenres, movieYears, movieTotalRatings, movieRatingCounts, moviePath)) {
        return false;
    }

    // 9. 생성 소요 시간 출력
    auto generationEnd = std::chrono::high_resolution_clock::now();
    double elapsedMs = std::chrono::duration_cast<std::chrono::microseconds>(generationEnd - generationStart).count() / 1000.0;
    std::cout << "[데이터 생성 소요 시간: " << elapsedMs << " ms]\n";

    // 10. 최소 평점 개수 검증
    size_t effectiveOkPercent = okPercent <= 0 ? PCT_SCALE : (size_t)okPercent;
    size_t minRatingCount = (ratingCnt * effectiveOkPercent + PCT_ROUND) / PCT_SCALE;
    if (minRatingCount < MIN_RATING) {
        minRatingCount = MIN_RATING;
    }

    return generated >= minRatingCount;
}

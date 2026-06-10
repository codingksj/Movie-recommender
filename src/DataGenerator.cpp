#include "DataGenerator.h"
#include <fstream>
#include "MovieConstant.h"
#include <random>
#include <set>
#include <unordered_set>
#include <vector>
#include <sstream>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <numeric>

using std::string;
using std::vector;
using namespace MC::Dataset;
using namespace MC::Generator;

vector<string> DataGenerator::titleAdj;
vector<string> DataGenerator::titleNoun;
vector<string> DataGenerator::genre;
vector<string> DataGenerator::userBase;
vector<string> DataGenerator::userSuffix;

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

static void partialShuffle(vector<size_t>& arr, size_t count, std::mt19937_64& eng) {
    if (count > arr.size()) {
        count = arr.size();
    }
    for (size_t i = 0; i < count; ++i) {
        std::uniform_int_distribution<size_t> dist(i, arr.size() - 1);
        std::swap(arr[i], arr[dist(eng)]);
    }
}

static string fmtScore(int tenths) {
    string s;
    s.reserve(4);
    s += char('0' + (tenths / 10));
    s += '.';
    s += char('0' + (tenths % 10));
    return s;
}

bool DataGenerator::loadSeeds(const string& filePath) {
    titleAdj.clear();
    titleNoun.clear();
    genre.clear();
    userBase.clear();
    userSuffix.clear();

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
            userBase.push_back(value);
        } else if (type == SEED_TYPE_SUFFIX) {
            userSuffix.push_back(value);
        } else if (type == SEED_TYPE_GENRE) {
            genre.push_back(value);
        } else if (type == SEED_TYPE_TITLE_ADJ) {
            titleAdj.push_back(value);
        } else if (type == SEED_TYPE_TITLE_NOUN) {
            titleNoun.push_back(value);
        }
    }

    return !userBase.empty() && !userSuffix.empty() && !genre.empty()
        && !titleAdj.empty() && !titleNoun.empty();
}

bool DataGenerator::validate(size_t movieCnt, size_t userCnt,
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

void DataGenerator::genMovies(std::mt19937_64& engine,
                              size_t movieCnt,
                              vector<string>& outMovies,
                              vector<string>& outGenres,
                              vector<int>& outYears) {
    outMovies.reserve(movieCnt);
    outGenres.reserve(movieCnt);
    outYears.reserve(movieCnt);

    std::uniform_int_distribution<size_t> adjDist(0, titleAdj.size() - 1);
    std::uniform_int_distribution<size_t> nounDist(0, titleNoun.size() - 1);
    std::uniform_int_distribution<size_t> genreDist(0, genre.size() - 1);
    std::uniform_int_distribution<size_t> patternDist(0, PATTERN_COUNT - 1);
    std::uniform_int_distribution<int> yearDist(MIN_YEAR, MAX_YEAR);

    const auto& romans = ROMANS;
    for (size_t i = 1; i <= movieCnt; ++i) {
        string title;
        const string& adj1 = titleAdj[adjDist(engine)];
        const string& adj2 = titleAdj[adjDist(engine)];
        const string& noun1 = titleNoun[nounDist(engine)];
        const string& noun2 = titleNoun[nounDist(engine)];

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
        outGenres.push_back(genre[genreDist(engine)]);
        outYears.push_back(yearDist(engine));
    }
}

bool DataGenerator::genUsers(std::mt19937_64& engine,
                             size_t userCnt,
                             const string& userPath,
                             vector<string>& outUsers) {
    outUsers.reserve(userCnt);

    std::uniform_int_distribution<size_t> baseDist(0, userBase.size() - 1);
    std::uniform_int_distribution<size_t> suffixDist(0, userSuffix.size() - 1);
    std::uniform_int_distribution<int> digitDist(DIGIT_MIN, DIGIT_MAX);

    std::unordered_set<string> usedUserNames;
    usedUserNames.reserve(userCnt * 2);

    std::ostringstream userBuf;
    userBuf << "userName,userEmail\n";

    for (size_t i = 1; i <= userCnt; ++i) {
        string userName;
        do {
            userName = userBase[baseDist(engine)]
                     + std::to_string(digitDist(engine))
                     + userSuffix[suffixDist(engine)];
        } while (!usedUserNames.insert(userName).second);

        outUsers.push_back(userName);
        userBuf << userName << ',' << userName << "@example.com\n";
    }

    std::ofstream userOut(userPath);
    if (!userOut.is_open()) {
        return false;
    }
    userOut << userBuf.str();
    return true;
}

void DataGenerator::buildPerMovieCnt(size_t movieCnt, size_t ratingCnt,
                                       vector<size_t>& outPerMovieCnt) {
    outPerMovieCnt.assign(movieCnt, 0);
    size_t base = ratingCnt / movieCnt;
    size_t extra = ratingCnt % movieCnt;
    for (size_t i = 0; i < movieCnt; ++i) {
        outPerMovieCnt[i] = base + (i < extra ? 1 : 0);
    }
}

bool DataGenerator::genRatings(std::mt19937_64& engine,
                               const RatingConfig& config,
                               RatingOutputs& outputs) {
    outputs.movieTotalRatings.assign(config.movieCnt, 0.0);
    outputs.movieRatingCounts.assign(config.movieCnt, 0);

    std::uniform_int_distribution<int> scoreDist(SCORE_TENTHS_MIN, SCORE_TENTHS_MAX);
    std::uniform_int_distribution<size_t> userDist(0, config.userCnt - 1);

    vector<size_t> userPool(config.userCnt);
    for (size_t i = 0; i < config.userCnt; ++i) {
        userPool[i] = i;
    }

    size_t totalRatings = 0;
    for (size_t cnt : config.perMovieCnt) {
        totalRatings += cnt;
    }

    string batch;
    batch.reserve(totalRatings * 48);
    batch += "userName,movieTitle,userRating\n";

    size_t generated = 0;
    for (size_t mi = 0; mi < config.movieCnt; ++mi) {
        size_t movieRatings = config.perMovieCnt[mi];
        if (movieRatings == 0) {
            continue;
        }

        size_t take = std::min(movieRatings, config.userCnt);
        partialShuffle(userPool, take, engine);

        const string& movieTitle = config.movies[mi];
        for (size_t t = 0; t < take; ++t) {
            int tenths = scoreDist(engine);
            double score = tenths / SCORE_SCALE;
            batch += config.users[userPool[t]];
            batch += ',';
            batch += movieTitle;
            batch += ',';
            batch += fmtScore(tenths);
            batch += '\n';
            outputs.movieTotalRatings[mi] += score;
            ++outputs.movieRatingCounts[mi];
            ++generated;
        }

        for (size_t extra = take; extra < movieRatings; ++extra) {
            int tenths = scoreDist(engine);
            double score = tenths / SCORE_SCALE;
            batch += config.users[userDist(engine)];
            batch += ',';
            batch += movieTitle;
            batch += ',';
            batch += fmtScore(tenths);
            batch += '\n';
            outputs.movieTotalRatings[mi] += score;
            ++outputs.movieRatingCounts[mi];
            ++generated;
        }
    }

    std::ofstream ratingOut(config.ratingPath);
    if (!ratingOut.is_open()) {
        return false;
    }
    ratingOut << batch;
    return generated > 0;
}

bool DataGenerator::saveMovies(const MovieSaveParams& params) {
    std::ostringstream movieBuf;
    movieBuf << "title,genre,year,totalRating,ratingCount\n";
    for (size_t mi = 0; mi < params.movies.size(); ++mi) {
        movieBuf << params.movies[mi] << ',' << params.genres[mi] << ','
                 << params.years[mi] << ',';
        movieBuf << params.totalRatings[mi] << ',' << params.ratingCounts[mi] << '\n';
    }

    std::ofstream movieOut(params.moviePath);
    if (!movieOut.is_open()) {
        return false;
    }
    movieOut << movieBuf.str();
    return true;
}

DataGenerator::GenResult DataGenerator::generate(const GenConfig& config) {
    GenResult result;
    auto start = std::chrono::high_resolution_clock::now();

    if (!validate(config.movieCnt, config.userCnt, config.ratingCnt, config.okPercent)) {
        return result;
    }

    if (!loadSeeds(SEED_FILE_PATH)) {
        std::cerr << "Seed file 로드를 실패했습니다: " << SEED_FILE_PATH << "\n";
        return result;
    }

    std::random_device rd;
    std::mt19937_64 engine(rd());

    vector<string> movies;
    vector<string> movieGenres;
    vector<int> movieYears;
    genMovies(engine, config.movieCnt, movies, movieGenres, movieYears);

    vector<string> users;
    if (!genUsers(engine, config.userCnt, config.userPath, users)) {
        auto end = std::chrono::high_resolution_clock::now();
        result.ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
        return result;
    }

    vector<size_t> perMovieCnt;
    buildPerMovieCnt(config.movieCnt, config.ratingCnt, perMovieCnt);

    vector<double> movieTotalRatings;
    vector<int> movieRatingCounts;
    RatingConfig rCfg{movies, users, perMovieCnt, config.movieCnt, config.userCnt, config.ratingPath};
    RatingOutputs rOuts{movieTotalRatings, movieRatingCounts};

    if (!genRatings(engine, rCfg, rOuts)) {
        auto end = std::chrono::high_resolution_clock::now();
        result.ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
        return result;
    }

    size_t generated = std::accumulate(movieRatingCounts.begin(), movieRatingCounts.end(), (size_t)0);

    MovieSaveParams saveParams{movies, movieGenres, movieYears, movieTotalRatings, movieRatingCounts, config.moviePath};
    if (!saveMovies(saveParams)) {
        auto end = std::chrono::high_resolution_clock::now();
        result.ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
        return result;
    }

    size_t effectiveOkPercent = config.okPercent <= 0 ? PCT_SCALE : (size_t)config.okPercent;
    size_t minRatingCount = (config.ratingCnt * effectiveOkPercent + PCT_ROUND) / PCT_SCALE;
    if (minRatingCount < MIN_RATING) {
        minRatingCount = MIN_RATING;
    }

    auto end = std::chrono::high_resolution_clock::now();
    result.ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
    result.ok = generated >= minRatingCount;
    return result;
}

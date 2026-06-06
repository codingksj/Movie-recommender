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

namespace {
    constexpr size_t PATTERN_ADJ_NOUN = 0;
    constexpr size_t PATTERN_ADJ_ADJ_NOUN = 1;
    constexpr size_t PATTERN_THE_NOUN_OF_ADJ_NOUN = 2;
    constexpr size_t PATTERN_NOUN_COLON_ADJ_NOUN = 3;
    constexpr size_t PATTERN_COUNT = 4;
    const string SEED_FILE_PATH = "data/generate_seed.csv";
    const string SEED_TYPE_BASE = "base";
    const string SEED_TYPE_SUFFIX = "suffix";
    const string SEED_TYPE_GENRE = "genre";

    string trimString(const string& value) {
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

    bool loadSeedPools(const string& filePath,
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
            } else if (type == "title_adj") {
                titleAdjectives.push_back(value);
            } else if (type == "title_noun") {
                titleNouns.push_back(value);
            }
        }

        return !baseWords.empty() && !suffixWords.empty() && !genres.empty()
            && !titleAdjectives.empty() && !titleNouns.empty();
    }

string escapeCsvField(const string& field) {
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
}

bool DataGenerator::generateSyntheticDataset(const string& movieOutputPath,
                                             const string& userOutputPath,
                                             const string& ratingOutputPath,
                                             size_t movieCount,
                                             size_t userCount,
                                             size_t ratingCount,
                                             int okPercent) {
    const size_t maxMovieCount = MovieConstants::MAX_MOVIE_COUNT;
    const size_t maxUserCount = MovieConstants::MAX_USER_COUNT;
    const size_t maxRatingCount = MovieConstants::MAX_RATING_COUNT;
    const int minYear = MovieConstants::DATA_MIN_YEAR;
    const int maxYear = MovieConstants::DATA_MAX_YEAR;

    if (movieCount == 0 || userCount == 0 || ratingCount == 0) {
        return false;
    }
    if (movieCount > maxMovieCount || userCount > maxUserCount || ratingCount > maxRatingCount) {
        return false;
    }
    if (okPercent < 0 || okPercent > MovieConstants::Generator::PCT_SCALE) {
        return false;
    }
    if (ratingCount > movieCount * userCount) {
        return false;
    }

    size_t effectiveOkPercent = okPercent <= 0 ? MovieConstants::Generator::PCT_SCALE : (size_t)okPercent;
    size_t minRatingCount = (ratingCount * effectiveOkPercent + MovieConstants::Generator::PCT_ROUND) / MovieConstants::Generator::PCT_SCALE;
    if (minRatingCount < MovieConstants::Generator::MIN_RATING) {
        minRatingCount = MovieConstants::Generator::MIN_RATING;
    }

    auto generationStart = std::chrono::high_resolution_clock::now();

    vector<string> titleAdjectives;
    vector<string> titleNouns;
    vector<string> userBases;
    vector<string> userSuffixes;
    vector<string> genres;
    if (!loadSeedPools(SEED_FILE_PATH, userBases, userSuffixes, genres, titleAdjectives, titleNouns)) {
        std::cerr << "Seed file 로드를 실패했습니다: " << SEED_FILE_PATH << "\n";
        return false;
    }

    // RNG and distributions
    std::random_device rd;
    std::mt19937_64 engine(rd());
    std::uniform_int_distribution<size_t> adjDist(0, titleAdjectives.size() - 1);
    std::uniform_int_distribution<size_t> nounDist(0, titleNouns.size() - 1);
    std::uniform_int_distribution<size_t> genreDist(0, genres.size() - 1);
    std::uniform_int_distribution<size_t> patternDist(0, PATTERN_COUNT - 1);
    std::uniform_int_distribution<int> yearDist(minYear, maxYear);
    std::normal_distribution<double> scoreNormDist(MovieConstants::Generator::Z_MEAN,
                                                      MovieConstants::Generator::Z_STDDEV); // standard normal

    // prepare movie meta (titles, genres, years)
    vector<string> movies;
    vector<string> movieGenres;
    vector<int> movieYears;
    movies.reserve(movieCount);
    movieGenres.reserve(movieCount);
    movieYears.reserve(movieCount);

    const auto &romans = MovieConstants::Generator::ROMANS;
    for (size_t i = 1; i <= movieCount; ++i) {
        string title;
        const string& adj1 = titleAdjectives[adjDist(engine)];
        const string& adj2 = titleAdjectives[adjDist(engine)];
        const string& noun1 = titleNouns[nounDist(engine)];
        const string& noun2 = titleNouns[nounDist(engine)];
        switch (patternDist(engine)) {
            case PATTERN_ADJ_NOUN:
                title = adj1 + " " + noun1;
                break;
            case PATTERN_ADJ_ADJ_NOUN:
                title = adj1 + " " + adj2 + " " + noun1;
                break;
            case PATTERN_THE_NOUN_OF_ADJ_NOUN:
                title = string("The ") + noun1 + " of " + adj1 + " " + noun2;
                break;
            case PATTERN_NOUN_COLON_ADJ_NOUN:
                title = noun1 + ": " + adj1 + " " + noun2;
                break;
            default:
                title = adj1 + " " + noun1;
                break;
        }
        // series number as 1..7; omit '1' (I) from title
        int series = (int)(((i - 1) % MovieConstants::Generator::SERIES_LENGTH) + 1);
        if (series > 1 && series <= MovieConstants::Generator::SERIES_LENGTH) {
            title += " " + romans[series - 1];
        }

        movies.push_back(title);
        movieGenres.push_back(genres[genreDist(engine)]);
        movieYears.push_back(yearDist(engine));
    }

    // user generation
    std::uniform_int_distribution<size_t> baseDist(0, userBases.size() - 1);
    std::uniform_int_distribution<size_t> suffixDist(0, userSuffixes.size() - 1);
    std::uniform_int_distribution<int> digitDist(MovieConstants::Generator::DIGIT_MIN, MovieConstants::Generator::DIGIT_MAX);

    vector<string> users;
    users.reserve(userCount);
    std::set<string> usedUserNames;
    std::ofstream userOut(userOutputPath);
    if (!userOut.is_open()) {
        return false;
    }
    userOut << "userName,userEmail\n";
    for (size_t i = 1; i <= userCount; ++i) {
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
        users.push_back(userName);
        userOut << escapeCsvField(userName) << ',' << escapeCsvField(email) << '\n';
    }

    // prepare rating output and per-movie aggregates
    std::ofstream ratingOut(ratingOutputPath);
    if (!ratingOut.is_open()) {
        return false;
    }
    ratingOut << "userName,movieTitle,userRating\n";

    // Create shuffled 1/0 distribution to assign counts per movie
    size_t blockSize = (ratingCount + movieCount - 1) / movieCount; // ceil
    size_t totalSlots = blockSize * movieCount;
    vector<int> flat(totalSlots, 0);
    for (size_t k = 0; k < ratingCount; ++k) {
        flat[k] = 1;
    }
    std::shuffle(flat.begin(), flat.end(), engine);

    vector<double> movieTotalRatings(movieCount, 0.0);
    vector<int> movieRatingCounts(movieCount, 0);

    // for sampling users without replacement
    vector<size_t> userIndices(userCount);
    for (size_t i = 0; i < userCount; ++i) {
        userIndices[i] = i;
    }

    size_t generated = 0;
    for (size_t mi = 0; mi < movieCount; ++mi) {
        size_t start = mi * blockSize;
        size_t end = start + blockSize;
        size_t movieRatings = 0;
        for (size_t s = start; s < end && s < totalSlots; ++s) {
            if (flat[s]) {
                ++movieRatings;
            }
        }
        if (movieRatings == 0) {
            continue;
        }

        // choose users (without replacement when possible)
        std::shuffle(userIndices.begin(), userIndices.end(), engine);
        size_t take = std::min(movieRatings, userCount);
        for (size_t t = 0; t < take; ++t) {
            size_t uidx = userIndices[t];
            double z = scoreNormDist(engine);
            double u = MovieConstants::Generator::CDF_HALF * 
                       (1.0 + std::erf(z / MovieConstants::Generator::SQRT2)); // CDF of standard normal
            double mapped = u * MovieConstants::Generator::SCORE_MAX; // map to 0..10
            int scoreInt = (int)std::round(mapped * MovieConstants::Generator::SCORE_SCALE); // tenths
            if (scoreInt < MovieConstants::Generator::SCORE_TENTHS_MIN) {
                scoreInt = MovieConstants::Generator::SCORE_TENTHS_MIN;
            }
            if (scoreInt > MovieConstants::Generator::SCORE_TENTHS_MAX) {
                scoreInt = MovieConstants::Generator::SCORE_TENTHS_MAX;
            }
            double score = scoreInt / MovieConstants::Generator::SCORE_SCALE;
            ratingOut << escapeCsvField(users[uidx]) << ',' << escapeCsvField(movies[mi]) << ',' << std::fixed << std::setprecision(1) << score << '\n';
            movieTotalRatings[mi] += score;
            ++movieRatingCounts[mi];
            ++generated;
        }
        // if we still need more (movieRatings > userCount), allow repeats
        for (size_t extra = take; extra < movieRatings; ++extra) {
            size_t uidx = engine() % userCount;
            double z = scoreNormDist(engine);
            double u = MovieConstants::Generator::CDF_HALF * 
                       (1.0 + std::erf(z / MovieConstants::Generator::SQRT2));
            double mapped = u * MovieConstants::Generator::SCORE_MAX;
            int scoreInt = (int)std::round(mapped * MovieConstants::Generator::SCORE_SCALE);
            if (scoreInt < MovieConstants::Generator::SCORE_TENTHS_MIN) {
                scoreInt = MovieConstants::Generator::SCORE_TENTHS_MIN;
            }
            if (scoreInt > MovieConstants::Generator::SCORE_TENTHS_MAX) {
                scoreInt = MovieConstants::Generator::SCORE_TENTHS_MAX;
            }
            double score = scoreInt / MovieConstants::Generator::SCORE_SCALE;
            ratingOut << escapeCsvField(users[uidx]) << ',' << escapeCsvField(movies[mi]) << ',' << std::fixed << std::setprecision(1) << score << '\n';
            movieTotalRatings[mi] += score;
            ++movieRatingCounts[mi];
            ++generated;
        }
    }

    // write movies.csv with aggregated totals
    std::ofstream movieOutFinal(movieOutputPath);
    if (!movieOutFinal.is_open()) {
        return false;
    }
    movieOutFinal << "title,genre,year,totalRating,ratingCount\n";
    for (size_t mi = 0; mi < movieCount; ++mi) {
        movieOutFinal << escapeCsvField(movies[mi]) << ',' << escapeCsvField(movieGenres[mi]) << ',' << movieYears[mi] << ','
                      << std::fixed << std::setprecision(1) << movieTotalRatings[mi] << ',' << movieRatingCounts[mi] << '\n';
    }

    auto generationEnd = std::chrono::high_resolution_clock::now();
    double elapsedMs = std::chrono::duration_cast<std::chrono::microseconds>(generationEnd - generationStart).count() / 1000.0;
    std::cout << "[데이터 생성 소요 시간: " << elapsedMs << " ms]\n";

    return generated >= minRatingCount;
}

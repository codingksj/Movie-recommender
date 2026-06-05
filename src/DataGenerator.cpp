#include "DataGenerator.h"
#include <fstream>
#include "MovieConstant.h"
#include <random>
#include <set>
#include <vector>

using std::string;
using std::vector;

namespace {
constexpr size_t PATTERN_ADJ_NOUN = 0;
constexpr size_t PATTERN_ADJ_ADJ_NOUN = 1;
constexpr size_t PATTERN_THE_NOUN_OF_ADJ_NOUN = 2;
constexpr size_t PATTERN_NOUN_COLON_ADJ_NOUN = 3;
constexpr size_t PATTERN_COUNT = 4;

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
    const size_t maxCount = MovieConstants::MAX_DATASET_COUNT;
    const int minYear = MovieConstants::DATA_MIN_YEAR;
    const int maxYear = MovieConstants::DATA_MAX_YEAR;
    const double minScore = MovieConstants::MIN_RATE;
    const double maxScore = MovieConstants::MAX_RATE;
    if (movieCount == 0 || userCount == 0 || ratingCount == 0) {
        return false;
    }
    if (movieCount > maxCount || userCount > maxCount || ratingCount > maxCount) {
        return false;
    }
    if (okPercent < 0 || okPercent > 100) {
        return false;
    }
    if (ratingCount > movieCount * userCount) {
        return false;
    }

    size_t minRatingCount = ratingCount;
    if (okPercent > 0) {
        size_t percentLeft = 100 - okPercent;
        size_t rounded = (ratingCount * percentLeft + 50) / 100;
        minRatingCount = rounded < 1 ? 1 : rounded;
    }

    const vector<string> adjectives = {
        "Dark", "Hidden", "Silent", "Lost", "Last", "First", "Secret", "Broken",
        "Golden", "Final", "Frozen", "Crimson", "Midnight", "Neon", "Shadowed",
        "Forgotten", "Burning", "Shattered", "Eternal", "Vanishing", "Silver",
        "Whispering", "Raging", "Hollow", "Desolate", "Radiant", "Bleak",
        "Cursed", "Sacred", "Distant", "Phantom"
    };
    const vector<string> nouns = {
        "Night", "Memory", "City", "Empire", "Shadow", "Journey", "Promise",
        "Legend", "Dream", "Storm", "Legacy", "Code", "Signal", "Horizon",
        "Motion", "Whisper", "Reflection", "Eclipse", "Abyss", "Odyssey",
        "Mirage", "Ritual", "Serpent", "Canvas", "Whirlpool", "Echo",
        "Voyage", "Sanctum", "Phantom", "Requiem"
    };
    const vector<string> genres = {"Action", "Drama", "Comedy", "Thriller", "Sci-Fi", "Fantasy", "Romance", "Horror", "Animation", "Mystery", "Adventure", "Noir"};

    std::random_device rd;
    std::mt19937_64 engine(rd());
    std::uniform_int_distribution<size_t> adjDist(0, adjectives.size() - 1);
    std::uniform_int_distribution<size_t> nounDist(0, nouns.size() - 1);
    std::uniform_int_distribution<size_t> genreDist(0, genres.size() - 1);
    std::uniform_int_distribution<size_t> patternDist(0, PATTERN_COUNT - 1);
    std::uniform_int_distribution<int> yearDist(minYear, maxYear);
    std::uniform_real_distribution<double> scoreDist(minScore, maxScore);

    vector<string> movies;
    movies.reserve(movieCount);
    std::ofstream movieOut(movieOutputPath);
    if (!movieOut.is_open()) {
        return false;
    }
    movieOut << "title,genre,year,totalRating,ratingCount\n";
    for (size_t i = 1; i <= movieCount; ++i) {
        string title;
        const string& adj1 = adjectives[adjDist(engine)];
        const string& adj2 = adjectives[adjDist(engine)];
        const string& noun1 = nouns[nounDist(engine)];
        const string& noun2 = nouns[nounDist(engine)];
        switch (patternDist(engine)) {
            case PATTERN_ADJ_NOUN: {
                // Adjective + Noun
                title = adj1 + " " + noun1 + " " + std::to_string(i);
                break;
            }
            case PATTERN_ADJ_ADJ_NOUN: {
                // Adjective + Adjective + Noun
                title = adj1 + " " + adj2 + " " + noun1 + " " + std::to_string(i);
                break;
            }
            case PATTERN_THE_NOUN_OF_ADJ_NOUN: {
                // The Noun of Adjective Noun
                title = string("The ") + noun1 + " of " + adj1 + " " + noun2 + " " + std::to_string(i);
                break;
            }
            case PATTERN_NOUN_COLON_ADJ_NOUN: {
                // Noun: Adjective Noun
                title = noun1 + ": " + adj1 + " " + noun2 + " " + std::to_string(i);
                break;
            }
            default: {
                title = adj1 + " " + noun1 + " " + std::to_string(i);
                break;
            }
        }
        string genre = genres[genreDist(engine)];
        int year = yearDist(engine);
        movies.push_back(title);
        movieOut << escapeCsvField(title) << ',' << escapeCsvField(genre) << ',' << year << ",0,0\n";
    }

    vector<string> users;
    users.reserve(userCount);
    std::ofstream userOut(userOutputPath);
    if (!userOut.is_open()) {
        return false;
    }
    userOut << "userName,userEmail\n";
    for (size_t i = 1; i <= userCount; ++i) {
        string userName = "User" + std::to_string(i);
        string email = "user" + std::to_string(i) + "@example.com";
        users.push_back(userName);
        userOut << escapeCsvField(userName) << ',' << escapeCsvField(email) << '\n';
    }

    std::ofstream ratingOut(ratingOutputPath);
    if (!ratingOut.is_open()) {
        return false;
    }
    ratingOut << "userName,movieTitle,userRating\n";

    std::uniform_int_distribution<size_t> userDist(0, users.size() - 1);
    std::uniform_int_distribution<size_t> movieDist(0, movies.size() - 1);
    std::set<std::pair<size_t, size_t>> usedPairs;

    size_t generated = 0;
    size_t attempts = 0;
    while (generated < ratingCount && attempts < ratingCount * 10) {
        size_t u = userDist(engine);
        size_t m = movieDist(engine);
        if (usedPairs.emplace(u, m).second) {
            double score = scoreDist(engine);
            ratingOut << escapeCsvField(users[u]) << ','
                      << escapeCsvField(movies[m]) << ','
                      << score << '\n';
            ++generated;
        }
        ++attempts;
    }

    return generated >= minRatingCount;
}

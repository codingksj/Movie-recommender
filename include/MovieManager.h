#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <utility>
#include "Movie.h"
#include "BaseManager.h"
#include "MovieConstant.h"

using std::vector;
using std::string;
using std::pair;

class MovieManager : public BaseManager {
private:
    vector<Movie> movies;

public:
    pair<MovieResult, double> addMovie(const string& title, int year, const string& genre);
    pair<vector<string>, double> searchByTitleFormatted(const string& query, string& outQuery) const;
    pair<vector<string>, double> getSortedMoviesFormatted(int sortChoice, string& outSortName) const;
    
    Movie* findMovieByTitle(const string& title);
    const Movie* findMovieByTitle(const string& title) const;
    const vector<Movie>& getMovies() const { return movies; }

    double loadFromFile() override;
    double saveToFile() override;
    int size() const override { return movies.size(); }
};

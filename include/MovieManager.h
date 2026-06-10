#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <utility>
#include "Movie.h"
#include "Rating.h"
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
    pair<vector<string>, double> getSortedMoviesFormatted(int choice, string& sortName) const;
    
    Movie* findMovieByTitle(const string& title);
    const Movie* findMovieByTitle(const string& title) const;
    const vector<Movie>& getMovies() const { return movies; }
    void applyRatings(const vector<Rating>& ratings);

    double loadFromFile() override;
    double saveToFile() override;
    int size() const override { return movies.size(); }
};

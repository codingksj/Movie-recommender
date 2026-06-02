#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <utility>
#include "Movie.h"
#include "BaseManager.h"
#include "MovieConstant.h"

class MovieManager : public BaseManager {
private:
    std::vector<Movie> movies;

public:
    std::pair<MovieResult, double> addMovie(const std::string& title, int year, const std::string& genre);
    std::pair<std::vector<std::string>, double> searchByTitleFormatted(const std::string& query, std::string& outQuery) const;
    std::pair<std::vector<std::string>, double> getSortedMoviesFormatted(int sortChoice, std::string& outSortName) const;
    
    Movie* findMovieByTitle(const std::string& title);
    const std::vector<Movie>& getMovies() const { return movies; }

    double loadFromFile() override;
    double saveToFile() override;
    int size() override { return movies.size(); }
};

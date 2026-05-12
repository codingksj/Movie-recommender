#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include "Movie.h"

class MovieManager {
private:
    std::vector<Movie> movies;

public:
    void addMovie();
    void searchByTitle();
    void printAllMovies();
    void printSortedByRating();
    Movie* findMovieByTitle(const std::string& title);
    const std::vector<Movie>& getMovies() const { return movies; }

    void loadMovies(const std::string& filename);
    void saveMovies(const std::string& filename);
};

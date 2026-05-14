#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include "Movie.h"
#include "BaseManager.h"

class MovieManager : public BaseManager {
private:
    std::vector<Movie> movies;

public:
    void addMovie();
    void searchByTitle();
    void printAllMovies();
    void printSortedByRating();
    Movie* findMovieByTitle(const std::string& title);
    const std::vector<Movie>& getMovies() const { return movies; }

    void loadFromFile() override;
    void saveToFile() override;
    int size() override { return movies.size(); }
};

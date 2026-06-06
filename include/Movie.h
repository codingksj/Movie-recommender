#pragma once
#include "Base.h"
#include "MovieConstant.h"

using std::ostream;

class Movie : public Base {
private:
    string      genre;
    int         releaseYear;
    double      totalRating;
    int         ratingCount;

public:
    Movie();
    Movie(const string& title, int year = MC::Dataset::MAX_YEAR, const string& genre = "unknown");
    Movie(const string& title, int year, const string& genre, double totalRating, int ratingCount);

    string      getTitle()           const;
    string      getGenre()           const;
    int         getReleaseYear()     const;
    double      getAverageRating()   const;
    int         getRatingCount()     const;
    bool        addRating(double r);    void        resetRatingAggregate();
    void        mergeRating(double r);
    bool operator>(const Movie& other) const;
    bool operator==(const Movie& other) const;
    bool operator==(const string& otherName) const override;

    friend ostream& operator<<(ostream& os, const Movie& m);
};

#include "Base.h"

class Movie : public Base {
private:
    string      genre;
    int         releaseYear;
    double      totalRating;
    int         ratingCount;

public:
    Movie();
    Movie(const string& title, int year = 2026, const string& genre = "unknown");
    Movie(const string& title, int year, const string& genre, double totalRating, int ratingCount);

    string      getTitle()           const;
    string      getGenre()           const;
    int         getReleaseYear()     const;
    double      getAverageRating()   const;
    int         getRatingCount()     const;
    bool        addRating(double r);

    bool operator>(const Movie& other) const;
    bool operator==(const Movie& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Movie& m);
};

#include "Base.h"

class Movie : public Base {
private:
    string      genre;
    int         releaseYear;
    double      totalRating;
    int         ratingCount;

public:
    Movie();
    Movie(const string& title, const string& genre, int year);

    string      getTitle()           const;
    string      getGenre()           const;
    int         getReleaseYear()     const;
    double      getAverageRating()   const;
    int         getRatingCount()     const;
    void        addRating(double r);
    friend std::ostream& operator<<(std::ostream& os, const Movie& m);
};

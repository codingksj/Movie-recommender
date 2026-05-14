#pragma once
#include "Base.h"

class Rating : public Base {
private:
    string  userName;
    double  userRating;

public:
    Rating(string userName, string movieTitle, double userRating);

    string     getUserName()        const;
    string     getMovieTitle()      const;
    double     getUserRating()      const;
    friend std::ostream& operator<<(std::ostream& os, const Rating& r);
};

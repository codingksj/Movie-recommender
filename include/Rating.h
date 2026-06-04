#pragma once
#include "Base.h"

using std::ostream;

class Rating : public Base {
private:
    string  userName;
    double  userRating;

public:
    Rating(string userName, string movieTitle, double userRating);

    string     getUserName()        const;
    string     getMovieTitle()      const;
    double     getUserRating()      const;
    friend ostream& operator<<(ostream& os, const Rating& r);
};

#pragma once
#include "Base.h"

using std::ostream;

class Rating : public Base {
private:
    string  userName;
    double  userRating;

public:
    using Base::operator==;
    Rating(const string& user, const string& title, double score);

    string     getUserName()        const;
    string     getMovieTitle()      const;
    double     getUserRating()      const;
    bool       operator==(const Rating& other) const;
    bool       operator==(const string& otherMovieTitle) const override;
    friend ostream& operator<<(ostream& os, const Rating& r);
};

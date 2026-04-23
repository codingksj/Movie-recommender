#include "rating.h"
#include <iostream>

using std::string;
using std::cout;

Rating::Rating(string userName, string movieTitle, double userRating)
    : Base(movieTitle) {
    this->userName = Base::validateString(userName, "유저 이름");
    if (userRating < 0.0 || userRating > 5.0) {
        this->userRating = 0.0;
    } else {
        this->userRating = userRating;
    }
}

string  Rating::getUserName()     const { return userName; }
string  Rating::getMovieTitle()   const { return Base::name; }
double  Rating::getUserRating()   const { return userRating; }

std::ostream& operator<<(std::ostream& os, const Rating& r) {
    os << "[ 유저 이름 ] " << r.userName << " [ 영화 제목 ] " << r.name << " [ 평점 ] " << r.userRating;
    return os;
}
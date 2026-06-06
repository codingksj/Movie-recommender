// 평점 클래스: 사용자-영화 관계와 점수 검증 담당

#include "Rating.h"
#include "MovieConstant.h"
#include <iostream>

using std::string;
using std::cout;

Rating::Rating(string userName, string movieTitle, double userRating)
    : Base(movieTitle) {
    this->userName = Base::validateString(userName, "유저 이름");
    if (userRating < MovieConstants::MIN_RATE || userRating > MovieConstants::MAX_RATE) {
        this->userRating = MovieConstants::MIN_RATE;
    } else {
        this->userRating = userRating;
    }
}

string  Rating::getUserName()     const { return userName; }

string  Rating::getMovieTitle()   const { return Base::name; }

double  Rating::getUserRating()   const { return userRating; }

bool Rating::operator==(const Rating& other) const {
    return Base::operator==(other);
}

bool Rating::operator==(const string& otherMovieTitle) const {
    return Base::operator==(otherMovieTitle);
}

std::ostream& operator<<(std::ostream& os, const Rating& r) {
    os << "[ 유저 이름 ] " << r.userName << " [ 영화 제목 ] " << r.name << " [ 평점 ] " << r.userRating;
    return os;
}
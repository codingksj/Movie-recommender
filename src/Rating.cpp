// 평점 도메인 모델 — 사용자·영화·점수 보관

#include "Rating.h"
#include "MovieConstant.h"

using namespace MC::Score;
#include <iostream>

using std::string;
using std::cout;

Rating::Rating(const string& user, const string& title, double score)
    : Base(title) {
    this->userName = Base::validateString(user, "유저 이름");
    if (score < MIN || score > MAX) {
        this->userRating = MIN;
    } else {
        this->userRating = score;
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
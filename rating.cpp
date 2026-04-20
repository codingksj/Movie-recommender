#include "rating.h"
#include <iostream>

using std::string;
using std::cout;

Rating::Rating() : Base("", ""), userRating(0.0) {}
Rating::Rating(string userId, string movieTitle, double userRating)
    : Base("", movieTitle) {
    
    this->id = Base::validateString(userId, "아이디");

    if (userRating < 0.0 || userRating > 5.0) {
        cout << "평점 범위가 잘못되었습니다!" << '\n';
        this->userRating = 0.0;
    } 
    else {
        this->userRating = userRating;
    }
}

double  Rating::getUserRating()   const { return userRating; }

void Rating::display() const {
    cout << "[ 유저 ID ] " << Base::id << " [ 영화 제목 ] " << Base::name << " [ 평점 ] " << userRating << '\n';
}
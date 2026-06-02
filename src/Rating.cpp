// 사용자별 영화 평점을 나타내는 Rating 클래스

#include "Rating.h"
#include "movieConstant.h"
#include <iostream>

using std::string;
using std::cout;

// 유저 이름, 영화 제목, 평점 점수를 받는 생성자
Rating::Rating(string userName, string movieTitle, double userRating)
    : Base(movieTitle) {
    this->userName = Base::validateString(userName, "유저 이름");
    if (userRating < MovieConstants::MIN_RATE || userRating > MovieConstants::MAX_RATE) {
        this->userRating = MovieConstants::MIN_RATE;
    } else {
        this->userRating = userRating;
    }
}

// 평점을 남긴 유저 이름 반환
string  Rating::getUserName()     const { return userName; }

// 평점이 입력된 영화 제목 반환
string  Rating::getMovieTitle()   const { return Base::name; }

// 유저가 부여한 평점 점수 반환
double  Rating::getUserRating()   const { return userRating; }

// 출력 스트림 연산자 오버로딩
std::ostream& operator<<(std::ostream& os, const Rating& r) {
    os << "[ 유저 이름 ] " << r.userName << " [ 영화 제목 ] " << r.name << " [ 평점 ] " << r.userRating;
    return os;
}
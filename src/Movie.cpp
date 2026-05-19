// 개별 영화 정보를 담는 Movie 클래스

#include "Movie.h"
#include <iostream>

using std::string;
using std::cout;

// 출시 연도 허용 범위 상수
const int MIN_YEAR = 1888;
const int MAX_YEAR = 2026;

// 평점 점수 범위 상수
const double MIN_RATE = 0.0;
const double MAX_RATE = 10.0;

// 기본 생성자
Movie::Movie() : Base(""), genre(""), releaseYear(0), totalRating(0.0), ratingCount(0) {}

// 기본값을 지원하는 편의 생성자
Movie::Movie(const string& title, int year, const string& genre)
    : Base(title), totalRating(0.0), ratingCount(0) {
    this->genre = Base::validateString(genre, "장르");
    if (year < MIN_YEAR || year > MAX_YEAR) {
        cout << year << "은(는) 올바른 출시 연도 범위(" << MIN_YEAR << "~" << MAX_YEAR << ")를 벗어납니다." << '\n';
        this->releaseYear = MAX_YEAR;
    } else {
        this->releaseYear = year;
    }
}

// 파일 로드용 생성자
Movie::Movie(const string& title, int year, const string& genre, double totalRating, int ratingCount)
    : Base(title), genre(genre), releaseYear(year), totalRating(totalRating), ratingCount(ratingCount) {}

// 영화 제목 반환
string      Movie::getTitle()        const { return Base::name; }

// 영화 장르 반환
string      Movie::getGenre()        const { return genre; }

// 영화 개봉 연도 반환
int         Movie::getReleaseYear()  const { return releaseYear; }

// 평점 등록 횟수 반환
int         Movie::getRatingCount()  const { return ratingCount; }

// 영화 평균 평점 반환
double      Movie::getAverageRating() const { return ratingCount > 0 ? totalRating / ratingCount : 0.0; }

// 평점 추가 등록
bool Movie::addRating(double r) {
    if (r < MIN_RATE || r > MAX_RATE) {
        cout << r << "은(는) 평점 범위(" << MIN_RATE << " ~ " << MAX_RATE << ")를 벗어납니다." << '\n';
        return false;
    }
    cout << '[' << Base::name << "] 에 평점 추가 : " << r << "점\n";
    totalRating += r;
    ratingCount++;
    return true;
}

// 동등 비교 연산자 오버로딩 (제목 기준 - 대소문자 무시)
bool Movie::operator==(const Movie& other) const {
    return Base::operator==(other);
}

// 출력 스트림 연산자 오버로딩
std::ostream& operator<<(std::ostream& os, const Movie& m) {
    os << " 제목: " << m.name 
       << " | 장르: " << m.genre 
       << " | 연도: " << m.releaseYear 
       << " | 평점: " << m.getAverageRating() 
       << " (" << m.ratingCount << "건)";
    return os;
}

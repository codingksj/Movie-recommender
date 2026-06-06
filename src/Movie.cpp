// 영화 도메인 모델 — 메타데이터 및 평점 집계

#include "Movie.h"
#include <iostream>

using std::string;
using std::cout;

#include "MovieConstant.h"

using namespace MC::Dataset;
using namespace MC::Score;

Movie::Movie() : Base(""), genre(""), releaseYear(0), totalRating(0.0), ratingCount(0) {}

Movie::Movie(const string& title, int year, const string& genre)
    : Base(title), totalRating(0.0), ratingCount(0) {
    this->genre = Base::validateString(genre, "장르");
    if (year < MIN_YEAR || year > MAX_YEAR) {
        cout << year << "은(는) 올바른 출시 연도 범위를 벗어납니다.\n";
        this->releaseYear = MAX_YEAR;
    } else {
        this->releaseYear = year;
    }
}

Movie::Movie(const string& title, int year, const string& genre, double totalRating, int ratingCount)
    : Base(title), genre(genre), releaseYear(year), totalRating(totalRating), ratingCount(ratingCount) {}

string      Movie::getTitle()        const { return Base::name; }
string      Movie::getGenre()        const { return genre; }
int         Movie::getReleaseYear()  const { return releaseYear; }
int         Movie::getRatingCount()  const { return ratingCount; }
double      Movie::getAverageRating() const { return ratingCount > 0 ? totalRating / ratingCount : 0.0; }

bool Movie::addRating(double r) {
    if (r < MIN || r > MAX) {
        cout << r << "은(는) 평점 범위를 벗어납니다.\n";
        return false;
    }
    cout << '[' << Base::name << "] 에 평점 추가 : " << r << "점\n";
    totalRating += r;
    ratingCount++;
    return true;
}

void Movie::resetRatingAggregate() {
    totalRating = 0.0;
    ratingCount = 0;
}

void Movie::mergeRating(double r) {
    if (r < MIN || r > MAX) {
        return;
    }
    totalRating += r;
    ratingCount++;
}

bool Movie::operator>(const Movie& other) const {
    return getAverageRating() > other.getAverageRating();
}

bool Movie::operator==(const Movie& other) const {
    return Base::operator==(other)
        && genre == other.genre
        && releaseYear == other.releaseYear;
}

bool Movie::operator==(const string& otherName) const {
    if (otherName.empty()) {
        return false;
    }
    string titleLower = Base::normalizeString(Base::name);
    string queryLower = Base::normalizeString(otherName);
    return titleLower.find(queryLower) != string::npos;
}

std::ostream& operator<<(std::ostream& os, const Movie& m) {
    os << " 제목: " << m.name 
       << " | 장르: " << m.genre 
       << " | 연도: " << m.releaseYear 
       << " | 평점: " << m.getAverageRating() 
       << " (" << m.ratingCount << "건)";
    return os;
}

#include "Movie.h"
#include <iostream>

using std::string;
using std::cout;

Movie::Movie() : Base(""), genre(""), releaseYear(0), totalRating(0.0), ratingCount(0) {}

Movie::Movie(const string& title, int year, const string& genre)
    : Base(title), totalRating(0.0), ratingCount(0) {
    this->genre = Base::validateString(genre, "장르");
    if (year < 1888 || year > 2026) {
        cout << year << "은(는) 올바른 출시 연도 범위(1888~2026)를 벗어납니다." << '\n';
        this->releaseYear = 2026;
    } else {
        this->releaseYear = year;
    }
}

string      Movie::getTitle()        const { return Base::name; }
string      Movie::getGenre()        const { return genre; }
int         Movie::getReleaseYear()  const { return releaseYear; }
int         Movie::getRatingCount()  const { return ratingCount; }
double      Movie::getAverageRating() const { return ratingCount > 0 ? totalRating / ratingCount : 0.0; }

bool Movie::addRating(double r) {
    if (r < 0.0 || r > 5.0) {
        cout << r << "은(는) 평점 범위(0.0 ~ 5.0)를 벗어납니다." << '\n';
        return false;
    }
    cout << '[' << Base::name << "] 에 평점 추가 : " << r << "점\n";
    totalRating += r;
    ratingCount++;
    return true;
}

bool Movie::operator>(const Movie& other) const {
    if (getAverageRating() != other.getAverageRating()) {
        return getAverageRating() > other.getAverageRating();
    }
    return name < other.name;
}

bool Movie::operator==(const Movie& other) const {
    return name == other.name;
}

std::ostream& operator<<(std::ostream& os, const Movie& m) {
    os << m.name << " (" << m.releaseYear << ")" << "  평점: " << m.getAverageRating() << " (" << m.ratingCount << "건)";
    return os;
}

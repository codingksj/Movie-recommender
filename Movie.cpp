#include "Movie.h"
#include <iostream>

using std::string;
using std::cout;

// 기본 생성자
Movie::Movie()
    : Base("", ""), genre(""), releaseYear(0), 
    totalRating(0.0), ratingCount(0) {}

// 4인자 생성자
Movie::Movie(string id, const string& title, const string& genre, int year)
    : Base(id, title), totalRating(0.0), ratingCount(0) {
    
    this->genre = Base::validateString(genre, "장르");

    if (year < 1888 || year > 2026) {
        cout << "출시 연도가 잘못되었습니다!" << '\n';
        this->releaseYear = 2026;
    } else {
        this->releaseYear = year;
    }
}

string      Movie::getGenre()        const { return genre; }
int         Movie::getReleaseYear()  const { return releaseYear; }
int         Movie::getRatingCount()  const { return ratingCount; }

double Movie::getAverageRating() const {
    return ratingCount > 0 ? totalRating / ratingCount : 0.0;
}

void Movie::addRating(double r) {
    if (r < 0.0 || r > 5.0) {
        cout << r << "점은 평점 범위가 잘못되었습니다!" << '\n';
        return;
    }
    cout << '[' << Base::name << "] 에 평점 추가 : " << r << "점\n";
    totalRating += r;
    ratingCount++;
}

void Movie::display() const {           
    cout << Base::id << ". " << Base::name << " (" << releaseYear << ")"
              << "  평점: " << getAverageRating()
              << " (" << ratingCount << "건)" << '\n';
}

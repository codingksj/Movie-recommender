#include "Movie.h"
#include "rating.h"
#include "user.h"
#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

using std::cout;
using std::vector;

void movieSimulation(vector<Movie>& movies) {
    cout << "#### 영화 정보 출력 ####" << '\n';
    movies.push_back(Movie(1, "기생충", "드라마", 2019));
    movies.push_back(Movie(2, "인터스텔라", "SF", 2014));
    movies.push_back(Movie(3, "범죄도시", "액션", 2017));
    movies[0].addRating(4.8);
    movies[1].addRating(4.9);

    movies[0].display();
    movies[1].display();

    movies[0].addRating(5.0);
    movies[0].addRating(6.0);   // 유효성 검사 → 무시됨

    cout << movies[0].getTitle() << ": "<< movies[0].getAverageRating() << '\n';
    // 기생충: 4.9  (4.8 + 5.0) / 2
    for (const auto& m : movies) {
        m.display();
    }
    int size = movies.size();
    for(int i = 0; i < 5; i++){
        int idx = rand() % size;
        double score = ((rand() % 101) - 50) / 10.0; //-5.0 ~ 5.0
        movies[idx].addRating(score);
    }
    for (const auto& m : movies) {
        m.display();
    }
    cout << "####################" << "\n\n";
}

void userSimulation(vector<User>& users){
    cout << "#### 유저 정보 출력 ####" << '\n';
    users.push_back(User("gildong123", "홍길동", "honggildong@gmail.com"));
    users.push_back(User("chulsoo456", "김철수", "imchulsu@naver.com"));

    for (const auto& u : users) {
        u.display();
    }
    cout << "####################" << "\n\n";
}

void ratingSimulation(vector<Rating>& ratings){
    cout << "#### 평점 정보 출력 ####" << '\n';
    ratings.push_back(Rating("gildong123", 1, 4.8));
    ratings.push_back(Rating("chulsoo456", 2, 4.9));
    for (const auto& r : ratings) {
        r.display();
    }
    cout << "####################" << "\n\n";
}

int main() {
    vector<Movie> movies;
    vector<User> users;
    vector<Rating> ratings;

    cout << std::setprecision(3);

    movieSimulation(movies);
    userSimulation(users);
    ratingSimulation(ratings);

    return 0;
}
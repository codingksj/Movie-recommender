#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include <iostream>
#include <iomanip>
using std::cout;
using std::cin;
void showMenu() {
    cout << "\n[ 영화 ]\n1. 영화추가\n2. 제목으로검색\n3. 전체목록출력\n4. 평점순정렬출력\n[ 사용자 ]\n5. 사용자추가\n6. 사용자목록출력\n[ 평점 ]\n7. 평점입력\n8. 영화별평점보기\n0. 종료\n선택> ";
}
int main() {
    MovieManager movieManager;
    UserManager userManager;
    RatingManager ratingManager;
    int choice;
    cout << std::fixed << std::setprecision(1);
    while (true) {
        showMenu();
        if (!(cin >> choice)) {
            if (cin.eof()) break;
            cin.clear(); cin.ignore(1000, '\n');
            cout << "올바른 숫자를 입력해주세요.\n";
            continue;
        }
        if (choice == 0) {
            cout << "프로그램을 종료합니다.\n";
            break;
        }
        switch (choice) {
            case 1: { movieManager.addMovie(); break; }
            case 2: { movieManager.searchByTitle(); break; }
            case 3: { movieManager.printAllMovies(); break; }
            case 4: { movieManager.printSortedByRating(); break; }
            case 5: { userManager.addUser(); break; }
            case 6: { userManager.printAllUsers(); break; }
            case 7: { ratingManager.addRating(movieManager, userManager); break; }
            case 8: { ratingManager.printRatingsByMovie(); break; }
            default: { cout << "잘못된 선택입니다.\n"; break; }
        }
    }
    return 0;
}
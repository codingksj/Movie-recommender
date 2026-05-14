#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include "Recommender.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using std::cout;
using std::cin;
using std::string;
using std::vector;

void showHeader(const string& title) {
    cout << "\n========================================\n";
    cout << "       " << title << "\n";
    cout << "========================================\n";
}

void showMainMenu() {
    showHeader("MOVIE RECOMMENDER SYSTEM");
    cout << " [1] 영화 관리\n";
    cout << " [2] 사용자 관리\n";
    cout << " [3] 평점 관리\n";
    cout << " [4] 추천 서비스\n";
    cout << " [0] 종료 및 저장\n";
    cout << "========================================\n";
    cout << "선택> ";
}

void showSubMenu(const string& title, const vector<string>& options) {
    showHeader(title);
    for (size_t i = 0; i < options.size(); ++i) {
        cout << " [" << (i + 1) << "] " << options[i] << "\n";
    }
    cout << " [0] 이전 메뉴로\n";
    cout << "========================================\n";
    cout << "선택> ";
}

int getChoice() {
    int choice;
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(1000, '\n');
        return -1;
    }
    return choice;
}

int main() {
    MovieManager movieManager;
    UserManager userManager;
    RatingManager ratingManager;
    Recommender recommender(movieManager, ratingManager, userManager);

    cout << std::fixed << std::setprecision(1);

    movieManager.setFilePath("data/movies.csv");
    userManager.setFilePath("data/users.csv");
    ratingManager.setFilePath("data/ratings.csv");

    movieManager.loadFromFile();
    userManager.loadFromFile();
    ratingManager.loadFromFile();

    bool running = true;
    while (running) {
        showMainMenu();
        int mainChoice = getChoice();

        if (mainChoice == 0) {
            cout << "데이터를 저장하고 프로그램을 종료합니다.\n";
            movieManager.saveToFile();
            userManager.saveToFile();
            ratingManager.saveToFile();
            running = false;
            break;
        }

        switch (mainChoice) {
            case 1: {
                while (true) {
                    showSubMenu("영화 관리", {"영화 추가", "제목으로 검색", "전체 목록 (정렬 포함)"});
                    int subChoice = getChoice();
                    if (subChoice == 0) {
                        break;
                    }
                    if (subChoice == 1) {
                        movieManager.addMovie();
                    } else if (subChoice == 2) {
                        movieManager.searchByTitle();
                    } else if (subChoice == 3) {
                        movieManager.printAllMovies();
                    } else {
                        cout << "잘못된 선택입니다.\n";
                    }
                }
                break;
            }
            case 2: {
                while (true) {
                    showSubMenu("사용자 관리", {"사용자 추가", "사용자 목록 출력"});
                    int subChoice = getChoice();
                    if (subChoice == 0) {
                        break;
                    }
                    if (subChoice == 1) {
                        userManager.addUser();
                    } else if (subChoice == 2) {
                        userManager.printAllUsers();
                    } else {
                        cout << "잘못된 선택입니다.\n";
                    }
                }
                break;
            }
            case 3: {
                while (true) {
                    showSubMenu("평점 관리", {"평점 입력", "영화별 평점 보기"});
                    int subChoice = getChoice();
                    if (subChoice == 0) {
                        break;
                    }
                    if (subChoice == 1) {
                        ratingManager.addRating(movieManager, userManager);
                    } else if (subChoice == 2) {
                        ratingManager.printRatingsByMovie();
                    } else {
                        cout << "잘못된 선택입니다.\n";
                    }
                }
                break;
            }
            case 4: {
                while (true) {
                    showSubMenu("추천 서비스", {"유사 사용자 찾기", "영화 추천 받기"});
                    int subChoice = getChoice();
                    if (subChoice == 0) {
                        break;
                    }
                    if (subChoice == 1) {
                        recommender.findSimilarUsers();
                    } else if (subChoice == 2) {
                        recommender.recommendMenu();
                    } else {
                        cout << "잘못된 선택입니다.\n";
                    }
                }
                break;
            }
            default: {
                cout << "잘못된 선택입니다.\n";
                break;
            }
        }
    }

    return 0;
}
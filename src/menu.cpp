// 메뉴 인터페이스 출력 및 제어 루프 기능 제공

#include "menu.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

// 메뉴 타이틀 상수
const string TITLE_MAIN = "MOVIE RECOMMENDER SYSTEM";
const string TITLE_MOVIE = "영화 관리";
const string TITLE_USER = "사용자 관리";
const string TITLE_RATING = "평점 관리";
const string TITLE_RECOMMEND = "추천 서비스";

// 메뉴 선택 옵션 리스트 상수
const vector<string> OPTIONS_MAIN = {
    "영화 관리",
    "사용자 관리",
    "평점 관리",
    "추천 서비스"
};

const vector<string> OPTIONS_MOVIE = {
    "영화 추가",
    "제목으로 검색",
    "전체 목록 (정렬 포함)"
};

const vector<string> OPTIONS_USER = {
    "사용자 추가",
    "사용자 목록 출력"
};

const vector<string> OPTIONS_RATING = {
    "평점 입력",
    "영화별 평점 보기"
};

const vector<string> OPTIONS_RECOMMEND = {
    "유사 사용자 찾기",
    "영화 추천 받기 (사용자 기준)",
    "영화 추천 받기 (장르 유사도 기준)"
};

// 화면 출력/동적 레이아웃 관련 상수
const size_t MIN_WIDTH = 40;
const size_t TITLE_PAD = 6;
const int IGN_LIMIT = 10000;

// 추천 알고리즘 설정 상수
const int SIM_LIMIT = 3;
const int COLLAB_K = 3;
const int COLLAB_N = 5;
const int GENRE_N = 5;

// 상단 타이틀 배너 출력
void showHeader(const string& title) {
    cout << "\n========================================\n";
    cout << "       " << title << "\n";
    cout << "========================================\n";
}

// 메인 메뉴 출력
void showMainMenu() {
    showHeader(TITLE_MAIN);
    for (size_t i = 0; i < OPTIONS_MAIN.size(); ++i) {
        cout << "  [" << (i + 1) << "] " << OPTIONS_MAIN[i] << "\n";
    }
    cout << "  [0] 종료 및 저장\n";
    cout << "========================================\n";
    cout << "선택> ";
}

// 서브 메뉴 출력
void showSubMenu(const string& title, const vector<string>& options) {
    showHeader(title);
    for (size_t i = 0; i < options.size(); ++i) {
        cout << "  [" << (i + 1) << "] " << options[i] << "\n";
    }
    cout << "  [0] 이전 메뉴로\n";
    cout << "========================================\n";
    cout << "선택> ";
}

// 다이나믹 너비 기준 결과 리스트 출력
void showDynamicResult(const string& title, const vector<string>& lines) {
    size_t max_len = title.length() + TITLE_PAD;
    for (const auto& line : lines) {
        if (line.length() > max_len) {
            max_len = line.length();
        }
    }

    if (max_len < MIN_WIDTH) {
        max_len = MIN_WIDTH;
    }

    string border(max_len + 4, '=');
    cout << "\n" << border << "\n";
    
    int padding = (static_cast<int>(max_len) + 4 - static_cast<int>(title.length())) / 2;
    if (padding < 0) padding = 0;
    cout << string(padding, ' ') << title << "\n";
    cout << border << "\n";

    for (const auto& line : lines) {
        cout << "  " << line << "\n";
    }
    cout << border << "\n";
}

// 사용자 선택 정수값 입력 받기
int getChoice() {
    int choice;
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(IGN_LIMIT, '\n');
        return -1;
    }
    return choice;
}

// 영화 관리 메뉴 처리 루프
void handleMovieMenu(MovieManager& movieManager) {
    while (true) {
        showSubMenu(TITLE_MOVIE, OPTIONS_MOVIE);
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
}

// 사용자 관리 메뉴 처리 루프
void handleUserMenu(UserManager& userManager) {
    while (true) {
        showSubMenu(TITLE_USER, OPTIONS_USER);
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
}

// 평점 관리 메뉴 처리 루프
void handleRatingMenu(RatingManager& ratingManager, MovieManager& movieManager, UserManager& userManager) {
    while (true) {
        showSubMenu(TITLE_RATING, OPTIONS_RATING);
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
}

// 추천 서비스 메뉴 처리 루프
void handleRecommendationMenu(Recommender& recommender) {
    while (true) {
        showSubMenu(TITLE_RECOMMEND, OPTIONS_RECOMMEND);
        int subChoice = getChoice();
        if (subChoice == 0) {
            break;
        }
        if (subChoice == 1) {
            string targetUser;
            cout << "유사 사용자를 찾을 사용자 이름 입력: ";
            cin >> targetUser;

            if (recommender.getUserManager().findUserByName(targetUser) == nullptr) {
                cout << "존재하지 않는 사용자입니다.\n";
                continue;
            }

            auto targetRatings = recommender.getUserRatings(targetUser);
            if (targetRatings.empty()) {
                cout << "해당 사용자의 평점 이력이 없어 유사도를 계산할 수 없습니다.\n";
                continue;
            }

            auto similarities = recommender.getSimilarUsers(targetUser, SIM_LIMIT);
            if (similarities.empty()) {
                cout << "유사한 사용자가 없습니다.\n";
                continue;
            }

            vector<string> lines;
            int count = 0;
            for (const auto& s : similarities) {
                stringstream ss;
                ss << "  " << ++count << ". " << s.first
                   << " (유사도: " << fixed << setprecision(3) << s.second << ")";
                lines.push_back(ss.str());
            }
            showDynamicResult(targetUser + "와 유사한 사용자 상위 " + to_string(SIM_LIMIT) + "명", lines);
        } else if (subChoice == 2) {
            string targetUser;
            cout << "추천을 받을 사용자 이름 입력: ";
            cin >> targetUser;

            if (recommender.getUserManager().findUserByName(targetUser) == nullptr) {
                cout << "존재하지 않는 사용자입니다.\n";
                continue;
            }

            vector<string> recommended = recommender.recommend(targetUser, COLLAB_K, COLLAB_N);
            if (recommended.empty()) {
                cout << "추천할 만한 새로운 영화가 없습니다.\n";
                continue;
            }

            vector<string> lines;
            for (size_t i = 0; i < recommended.size(); ++i) {
                lines.push_back("  " + to_string(i + 1) + ". " + recommended[i]);
            }
            showDynamicResult(targetUser + "님을 위한 추천 영화", lines);
        } else if (subChoice == 3) {
            string title;
            cout << "기준 영화 제목 입력: ";
            cin.ignore();
            getline(cin, title);

            vector<string> recommended = recommender.recommendByGenre(title, GENRE_N);
            if (recommended.empty()) {
                cout << "추천할 비슷한 장르의 영화가 없거나 존재하지 않는 영화입니다.\n";
                continue;
            }

            vector<string> lines;
            for (size_t i = 0; i < recommended.size(); ++i) {
                lines.push_back("  " + to_string(i + 1) + ". " + recommended[i]);
            }
            showDynamicResult(title + "와(과) 비슷한 장르 추천 영화", lines);
        } else {
            cout << "잘못된 선택입니다.\n";
        }
    }
}

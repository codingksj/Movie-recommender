#include "SimilarityCalculator.h"
#include <cstdlib> // std::abs
int SimilarityCalculator::calculate(
    const std::vector<Rating>& user1,
    const std::vector<Rating>& user2)
{
int commonCount = 0; // 공통으로본영화수
int scoreDiffSum = 0; // 평점차이의합
for (const Rating& r1 : user1) {
for (const Rating& r2 : user2) {
        if (r1.getMovieTitle() == r2.getMovieTitle()) {
            commonCount++;
            scoreDiffSum += std::abs(r1.getUserRating() - r2.getUserRating());
        }
}
}
// 공통영화가없으면비교자체가불가능→ 매우낮은점수
if (commonCount == 0) {
return -100;
}
// 우리의단순유사도공식
return commonCount * 10 -scoreDiffSum;
}
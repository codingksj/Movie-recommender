#include "Base.h"

class Movie : public Base {
private:
    string      genre;
    int         releaseYear;
    double      totalRating;
    int         ratingCount;

public:
    Movie();
    Movie(string id, const string& title,
          const string& genre, int year);

    // getId()와 getTitle()은 Base에서 상속받은 getId(), getName()으로 대체 가능하지만,
    // 기존 하위 호환성을 위해 유지하거나 Base 메서드를 호출하도록 수정
    string      getId()              const { return Base::id; }
    string      getTitle()           const { return Base::name; }
    string      getGenre()           const;
    int         getReleaseYear()     const;
    double      getAverageRating()   const;
    int         getRatingCount()     const;
    void        addRating(double r);
    void        display()            const override;
};

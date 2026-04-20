#include "Base.h"

class Rating : public Base {
private:
    double  userRating;

public:
    Rating();
    Rating(string userId, string movieTitle, double userRating);

    string     getUserId()          const { return Base::id; }
    string     getMovieTitle()      const { return Base::name; }
    double     getUserRating()      const;
    void       display()            const override;
};

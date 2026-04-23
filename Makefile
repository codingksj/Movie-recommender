# Makefile — Movie 클래스 빌드
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -g
TARGET   = movie-app
OBJS     = main.o Movie.o user.o rating.o Base.o MovieManager.o UserManager.o RatingManager.o

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp MovieManager.h UserManager.h RatingManager.h
	$(CXX) $(CXXFLAGS) -c $<

Movie.o: Movie.cpp Movie.h Base.h
	$(CXX) $(CXXFLAGS) -c $<

user.o: user.cpp user.h Base.h
	$(CXX) $(CXXFLAGS) -c $<

rating.o: rating.cpp rating.h Base.h
	$(CXX) $(CXXFLAGS) -c $<

Base.o: Base.cpp Base.h
	$(CXX) $(CXXFLAGS) -c $<

MovieManager.o: MovieManager.cpp MovieManager.h Movie.h
	$(CXX) $(CXXFLAGS) -c $<

UserManager.o: UserManager.cpp UserManager.h user.h
	$(CXX) $(CXXFLAGS) -c $<

RatingManager.o: RatingManager.cpp RatingManager.h rating.h MovieManager.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean run
clean:
	rm -f $(OBJS) $(TARGET)
run: $(TARGET)
	./$(TARGET)
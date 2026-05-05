# Makefile — Movie 클래스 빌드
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -g -Isrc
TARGET   = movie-app
SRC_DIR  = src
OBJS     = main.o Movie.o user.o rating.o Base.o MovieManager.o UserManager.o RatingManager.o SimilarityCalculator.o

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/MovieManager.h $(SRC_DIR)/UserManager.h $(SRC_DIR)/RatingManager.h
Movie.o: $(SRC_DIR)/Movie.cpp $(SRC_DIR)/Movie.h $(SRC_DIR)/Base.h
user.o: $(SRC_DIR)/user.cpp $(SRC_DIR)/user.h $(SRC_DIR)/Base.h
rating.o: $(SRC_DIR)/rating.cpp $(SRC_DIR)/rating.h $(SRC_DIR)/Base.h
Base.o: $(SRC_DIR)/Base.cpp $(SRC_DIR)/Base.h
MovieManager.o: $(SRC_DIR)/MovieManager.cpp $(SRC_DIR)/MovieManager.h $(SRC_DIR)/Movie.h
UserManager.o: $(SRC_DIR)/UserManager.cpp $(SRC_DIR)/UserManager.h $(SRC_DIR)/user.h
RatingManager.o: $(SRC_DIR)/RatingManager.cpp $(SRC_DIR)/RatingManager.h $(SRC_DIR)/rating.h $(SRC_DIR)/MovieManager.h
SimilarityCalculator.o: $(SRC_DIR)/SimilarityCalculator.cpp $(SRC_DIR)/SimilarityCalculator.h $(SRC_DIR)/rating.h

.PHONY: clean run
clean:
	rm -f $(OBJS) $(TARGET)
run: $(TARGET)
	./$(TARGET)
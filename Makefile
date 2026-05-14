# Makefile — Movie Recommender System (M3)
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -g -Isrc
TARGET   = movie-app
SRC_DIR  = src
OBJ_DIR  = obj
OBJS     = $(addprefix $(OBJ_DIR)/, main.o Movie.o user.o rating.o Base.o MovieManager.o UserManager.o RatingManager.o Recommender.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/MovieManager.h $(SRC_DIR)/UserManager.h $(SRC_DIR)/RatingManager.h $(SRC_DIR)/Recommender.h $(SRC_DIR)/BaseManager.h
$(OBJ_DIR)/Movie.o: $(SRC_DIR)/Movie.cpp $(SRC_DIR)/Movie.h $(SRC_DIR)/Base.h
$(OBJ_DIR)/user.o: $(SRC_DIR)/user.cpp $(SRC_DIR)/user.h $(SRC_DIR)/Base.h
$(OBJ_DIR)/rating.o: $(SRC_DIR)/rating.cpp $(SRC_DIR)/rating.h $(SRC_DIR)/Base.h
$(OBJ_DIR)/Base.o: $(SRC_DIR)/Base.cpp $(SRC_DIR)/Base.h
$(OBJ_DIR)/MovieManager.o: $(SRC_DIR)/MovieManager.cpp $(SRC_DIR)/MovieManager.h $(SRC_DIR)/Movie.h $(SRC_DIR)/BaseManager.h
$(OBJ_DIR)/UserManager.o: $(SRC_DIR)/UserManager.cpp $(SRC_DIR)/UserManager.h $(SRC_DIR)/user.h $(SRC_DIR)/BaseManager.h
$(OBJ_DIR)/RatingManager.o: $(SRC_DIR)/RatingManager.cpp $(SRC_DIR)/RatingManager.h $(SRC_DIR)/rating.h $(SRC_DIR)/MovieManager.h $(SRC_DIR)/BaseManager.h
$(OBJ_DIR)/Recommender.o: $(SRC_DIR)/Recommender.cpp $(SRC_DIR)/Recommender.h $(SRC_DIR)/MovieManager.h $(SRC_DIR)/UserManager.h $(SRC_DIR)/RatingManager.h

.PHONY: clean run
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
run: $(TARGET)
	./$(TARGET)
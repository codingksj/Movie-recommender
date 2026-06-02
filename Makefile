CXX      = g++
CXXFLAGS = -std=c++17 -Wall -g -Iinclude
TARGET   = main
SRC_DIR  = src
INC_DIR  = include
OBJ_DIR  = obj
OBJS     = $(addprefix $(OBJ_DIR)/, main.o Movie.o User.o Rating.o Base.o MovieManager.o UserManager.o RatingManager.o Recommender.o Menu.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(INC_DIR)/MovieManager.h $(INC_DIR)/UserManager.h $(INC_DIR)/RatingManager.h $(INC_DIR)/Recommender.h $(INC_DIR)/BaseManager.h $(INC_DIR)/Menu.h
$(OBJ_DIR)/Movie.o: $(SRC_DIR)/Movie.cpp $(INC_DIR)/Movie.h $(INC_DIR)/Base.h
$(OBJ_DIR)/User.o: $(SRC_DIR)/User.cpp $(INC_DIR)/User.h $(INC_DIR)/Base.h
$(OBJ_DIR)/Rating.o: $(SRC_DIR)/Rating.cpp $(INC_DIR)/Rating.h $(INC_DIR)/Base.h
$(OBJ_DIR)/Base.o: $(SRC_DIR)/Base.cpp $(INC_DIR)/Base.h
$(OBJ_DIR)/MovieManager.o: $(SRC_DIR)/MovieManager.cpp $(INC_DIR)/MovieManager.h $(INC_DIR)/Movie.h $(INC_DIR)/BaseManager.h $(INC_DIR)/Menu.h
$(OBJ_DIR)/UserManager.o: $(SRC_DIR)/UserManager.cpp $(INC_DIR)/UserManager.h $(INC_DIR)/User.h $(INC_DIR)/BaseManager.h $(INC_DIR)/Menu.h
$(OBJ_DIR)/RatingManager.o: $(SRC_DIR)/RatingManager.cpp $(INC_DIR)/RatingManager.h $(INC_DIR)/Rating.h $(INC_DIR)/MovieManager.h $(INC_DIR)/BaseManager.h $(INC_DIR)/Menu.h
$(OBJ_DIR)/Recommender.o: $(SRC_DIR)/Recommender.cpp $(INC_DIR)/Recommender.h $(INC_DIR)/MovieManager.h $(INC_DIR)/UserManager.h $(INC_DIR)/RatingManager.h
$(OBJ_DIR)/Menu.o: $(SRC_DIR)/Menu.cpp $(INC_DIR)/Menu.h $(INC_DIR)/MovieManager.h $(INC_DIR)/UserManager.h $(INC_DIR)/RatingManager.h $(INC_DIR)/Recommender.h $(INC_DIR)/movieConstant.h

.PHONY: clean run valgrind
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
run: $(TARGET)
	./$(TARGET)
valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)
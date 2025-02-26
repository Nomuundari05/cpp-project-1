CXX = g++
CXXFLAGS = -std=c++11 -w -g -Iinclude


SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin


SRC = $(SRC_DIR)/main.cpp $(SRC_DIR)/LongNumber.cpp
OBJ = $(SRC:.cpp=.o)


EXEC = $(BIN_DIR)/program

all: $(EXEC)


$(EXEC): $(OBJ) | $(BIN_DIR)
		$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC)


$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
		$(CXX) $(CXXFLAGS) -c $< -o $@


$(BIN_DIR):
		mkdir -p $(BIN_DIR)


clean:
		rm -f $(SRC_DIR)/*.o $(EXEC)

run: $(EXEC)
		clear
		./$(EXEC)
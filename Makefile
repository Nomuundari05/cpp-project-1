# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -w -g -Iinclude

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin

# Source files
SRC = $(SRC_DIR)/main.cpp $(SRC_DIR)/LongNumber.cpp
OBJ = $(SRC:.cpp=.o)

# Executable name
EXEC = $(BIN_DIR)/program

# Default target
all: $(EXEC)

# Rule for building the executable
$(EXEC): $(OBJ) | $(BIN_DIR)
		$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC)

# Rule for compiling .cpp files to .o object files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
		$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure bin directory exists
$(BIN_DIR):
		mkdir -p $(BIN_DIR)

# Clean rule
clean:
		rm -f $(SRC_DIR)/*.o $(EXEC)

# Run rule
run: $(EXEC)
		clear
		./$(EXEC)
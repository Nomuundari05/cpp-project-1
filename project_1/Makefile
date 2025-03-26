CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -I/opt/homebrew/include -I/opt/homebrew/Cellar/boost/1.87.0/include
LDFLAGS = -L/opt/homebrew/Cellar/boost/1.87.0/lib -lboost_system -lboost_filesystem -lboost_program_options

TARGET_LIB = liblongnumber.a
TARGET_PI = pi_calculator
TARGET_TEST = test_runner

SRCS = long_number.cpp
OBJS = $(SRCS:.cpp=.o)

PI_SRC = pi_calculator.cpp
PI_OBJ = $(PI_SRC:.cpp=.o)

TEST_DIR = tests
TEST_SRCS = $(TEST_DIR)/test_long_number.cpp $(TEST_DIR)/test_runner.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

all: $(TARGET_LIB) $(TARGET_PI) test

$(TARGET_LIB): $(OBJS)
	ar rcs $(TARGET_LIB) $(OBJS)

$(TARGET_PI): $(PI_OBJ) $(OBJS) $(TARGET_LIB)
	$(CXX) $(CXXFLAGS) -o $(TARGET_PI) $(PI_OBJ) $(OBJS) $(TARGET_LIB) $(LDFLAGS)


test: $(TEST_OBJS) $(TARGET_LIB)
	$(CXX) $(CXXFLAGS) -o $(TARGET_TEST) $(TEST_OBJS) $(TARGET_LIB) $(LDFLAGS)

run_tests: test
	./$(TARGET_TEST)

pi: $(TARGET_PI)
	./$(TARGET_PI) 100

clean:
	rm -f $(OBJS) $(PI_OBJ) $(TEST_OBJS) $(TARGET_LIB) $(TARGET_PI) $(TARGET_TEST)

.PHONY: all test run_tests pi clean

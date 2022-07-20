# compiler variables and flags
CXX=g++
# -02 for opt
CXXFLAGS=-Wall -g -std=c++11
# command variabels
RM=rm
RMFLAGS=-rf

# directories
BUILD_DIR=build
INCL_DIR=include

MAIN_BUILD_DIR=$(BUILD_DIR)/main
OBJ_DIR=$(MAIN_BUILD_DIR)/obj
BIN_DIR=$(MAIN_BUILD_DIR)/bin
SRC_DIR=src

# files
SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
# SRC_OBJS=$(SOURCES:%.c=$(OBJ_DIR)/%.o)
SRC_OBJS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

HEADERS=$(wildcard $(SRC_DIR)/*.hpp)
INCLUDES=$(wildcard $(INCL_DIR)/*.hpp)

# targets
MAIN_TARGET=$(BIN_DIR)/main

all: $(MAIN_TARGET)

$(MAIN_TARGET): $(SRC_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDES) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	$(RM) $(RMFLAGS) ./$(BUILD_DIR)

# TEST_ARGS=-b true --fname piplup.gif
# TEST_ARGS=-b true --
TEST_ARGS=1 2 3 --boolean blah
# TEST_ARGS=true piplup.gif 15 36 45 66
# @ suppresses the command output (but not the program's output)
run:
	./$(MAIN_TARGET) $(TEST_ARGS)

leaks:
	@lldb ./$(MAIN_TARGET) $(TEST_ARGS)

# TEST TARGETS / VARIABLES
TEST_BUILD_DIR=$(BUILD_DIR)/test
TEST_OBJ_DIR=$(TEST_BUILD_DIR)/obj
TEST_BIN_DIR=$(TEST_BUILD_DIR)/bin
TEST_SRC_DIR=test

# files
TEST_SOURCES=$(wildcard $(TEST_SRC_DIR)/*.cpp)
TEST_HEADERS=qtest.hpp
# SRC_OBJS=$(SOURCES:%.c=$(TEST_OBJ_DIR)/%.o)
TEST_SRC_OBJS=$(patsubst $(TEST_SRC_DIR)/%.cpp, $(TEST_OBJ_DIR)/%.o, $(TEST_SOURCES))

TEST_TARGET=$(TEST_BIN_DIR)/test

test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC_OBJS) | $(TEST_BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp $(INCLUDES) | $(TEST_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# $(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp | $(TEST_OBJ_DIR)
# 	$(CXX) $(CXXFLAGS) -o $@ -c $<

run-test:
	@./$(TEST_TARGET)

clean-test:
	$(RM) $(RMFLAGS) ./$(TEST_BUILD_DIR)

leaks-test:
	@lldb ./$(TEST_TARGET)

# create the .o and main executable directories as well as the
#  test binary directory if they don't exist.
$(BUILD_DIR) $(TEST_BUILD_DIR) $(MAIN_BUILD_DIR) $(OBJ_DIR) $(BIN_DIR) $(TEST_OBJ_DIR) $(TEST_BIN_DIR) $(INCL_DIR):
	@mkdir -p $@

.PHONY: clean all run leaks run-test test clean-test
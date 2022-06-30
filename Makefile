# compiler variables and flags
CXX=g++
# -02 for opt
CXXFLAGS=-Wall -g -std=c++20
# command variabels
RM=rm
RMFLAGS=-rf

# directories
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/obj
BIN_DIR=$(BUILD_DIR)/bin
SRC_DIR=src

# files
SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
# SRC_OBJS=$(SOURCES:%.c=$(OBJ_DIR)/%.o)
SRC_OBJS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
HEADERS=$(wildcard $(SRC_DIR)/*.hpp)

# targets
MAIN_TARGET=$(BIN_DIR)/main

all: $(MAIN_TARGET)

$(MAIN_TARGET): $(SRC_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCL_FLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCL_FLAGS) -o $@ -c $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCL_FLAGS) -o $@ -c $<

clean:
	$(RM) $(RMFLAGS) ./$(BUILD_DIR) ./lib

TEST_ARGS=-f true piplup.gif -p 9
# @ suppresses the command output (but not the program's output)
run:
	@./$(MAIN_TARGET) $(TEST_ARGS)

# create the .o and main executable directories as well as the
#  test binary directory if they don't exist.
$(BUILD_DIR) $(OBJ_DIR) $(BIN_DIR) $(INCL_DIR) $(LIB_DIR):
	mkdir -p $@

.PHONY: clean all run

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Iinclude

SRC_DIR = src
OBJ_DIR = obj

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

EXEC = n_puzzle

all: $(OBJ_DIR) $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(EXEC)

re: fclean all

.PHONY: clean fclean re

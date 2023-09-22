CXX = g++
CXXFLAGS = -std=c++20 -Wall -Iinclude

SRC_DIR = src
OBJ_DIR = obj

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

EXEC = n_puzzle

# WebAssembly settings
WASM_DIR = wasm
WASM_EXEC = n_puzzle_wasm
WASM_FLAGS = -s WASM=1 -s "EXPORTED_RUNTIME_METHODS=['cwrap']"

all: $(OBJ_DIR) $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# WebAssembly targets
app: $(WASM_DIR)
	emcc $(CXXFLAGS) $(WASM_FLAGS) -o $(WASM_DIR)/$(WASM_EXEC).js $(SOURCES)
	cp -R $(WASM_DIR) app/public/wasm

$(WASM_DIR):
	mkdir -p $(WASM_DIR) 

clean:
	rm -rf $(OBJ_DIR) $(WASM_DIR)

fclean: clean
	rm -rf $(EXEC)

re: fclean all

.PHONY: clean fclean re web

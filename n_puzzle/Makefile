CXX = g++
CXXFLAGS = -std=c++20 -Wall -Iincludes

SRCS_DIR = srcs
OBJS_DIR = objs

SOURCES = $(wildcard srcs/*.cpp)
OBJECTS = $(SOURCES:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)

EXEC = n_puzzle

# WebAssembly settings
WASM_DIR = ../public/wasm
WASM_EXEC = n_puzzle_wasm
WASM_FLAGS = -s WASM=1 -s "EXPORTED_RUNTIME_METHODS=['callMain']" -s INVOKE_RUN=0 -sALLOW_MEMORY_GROWTH=1 -s EXIT_RUNTIME=1 

# all: $(OBJS_DIR) $(EXEC)
all : app

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

app: $(WASM_DIR)
	emcc $(CXXFLAGS) $(WASM_FLAGS) -o $(WASM_DIR)/$(WASM_EXEC).js $(SOURCES) -O3

$(WASM_DIR):
	mkdir -p $(WASM_DIR) 

clean:
	rm -rf $(OBJS_DIR) $(WASM_DIR)

fclean: clean
	rm -rf $(EXEC)

re: fclean all

.PHONY: clean fclean re web
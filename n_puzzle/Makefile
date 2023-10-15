CXX = g++
CXXFLAGS = -std=c++20 -Wall -Iincludes

SRCS_DIR = srcs
OBJS_DIR = objs

SOURCES = $(wildcard srcs/*.cpp)

WASM_DIR = ../public/wasm
WASM_EXEC = n_puzzle_wasm
WASM_FLAGS = -s WASM=1 -s "EXPORTED_RUNTIME_METHODS=['callMain']" -s INVOKE_RUN=0 -sALLOW_MEMORY_GROWTH=1 -s EXIT_RUNTIME=1 

all : $(WASM_DIR)
	emcc $(CXXFLAGS) $(WASM_FLAGS) -o $(WASM_DIR)/$(WASM_EXEC).js $(SOURCES) -O3

$(WASM_DIR):
	mkdir -p $(WASM_DIR) 

clean:
	rm -rf $(WASM_DIR)

re: clean all

.PHONY: clean fclean re web
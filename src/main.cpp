#include "npuzzle.hpp"

// launch 'make && <test ./n-puzzle'

int main(int ac, char **av) {
    // Smart Pointer
    std::unique_ptr<Puzzle> puzzle = parse();

    // Check if it is well parsed
    std::cout << *puzzle << std::endl;
}
#include "npuzzle.hpp"
#include "utils.hpp"

// TODO understand algorithm and figure out how to parse string into Graph
// For now, I went for the obvious way, parsing the puzzle into a grid.

std::unique_ptr<Puzzle> parse() {
    std::unique_ptr<Puzzle> res;
    std::string line, puzzleStr;
    int rowCount = 0;

    // Treat line by line
    while (std::getline(std::cin, line)) {
        if (line[0] == '#') continue; // commentary

        // Puzzle not yet initialized, which means the first line after commentaries,
        // so coming the size information of the puzzle
        if (!res) { 
            res = std::make_unique<Puzzle>(std::stoi(line)); // puzzle constructor with size
            continue;
        }

        // Put the row string directly into the grid after split
        auto rowVals = split(line);
        for (int i = 0; i < res->size; i++)
            res->grid[rowCount][i] = std::stoi(rowVals[i]);
        rowCount++;
    }
    return res;
}

    

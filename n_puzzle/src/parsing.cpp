#include "puzzle.hpp"

unique_ptr<Puzzle> parse(const int ac, string puzzleStr) {
    unique_ptr<Puzzle> res;

    auto cells = split(puzzleStr);
    int size = sqrt(cells.size());
    res = make_unique<Puzzle>(size);
    for (int i = 0; i < cells.size(); i++) {
        res->grid[i / size][i % size] = stoi(cells[i]);
        res->initialGrid.push_back(stoi(cells[i]));
    }
    return res;
}


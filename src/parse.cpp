#include "puzzle.hpp"

unique_ptr<Puzzle> parse(const string &filename) {
    unique_ptr<Puzzle> res;
    string line, puzzleStr;
    int rowCount = 0;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    while (getline(file, line)) {        
        if (line[0] == '#') continue;

        if (!res) { 
            res = make_unique<Puzzle>(stoi(line));
            continue;
        }

        auto rowVals = split(line);
        for (int i = 0; i < res->size; i++) {
            res->grid[rowCount][i] = stoi(rowVals[i]);
            res->initialGrid.push_back(stoi(rowVals[i]));
        }
        rowCount++;
    }
    return res;
}


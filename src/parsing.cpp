#include "puzzle.hpp"

unique_ptr<Puzzle> parse(const int ac, const char *av[]) {
    unique_ptr<Puzzle> res;

    if (ac == 1) {
        auto grid = generate(4);
        res = make_unique<Puzzle>(4);
        res->grid = grid;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                res->initialGrid.push_back(grid[i][j]);
        }
        return res;
    }

    const string filename = string(av[1]);

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


#include "headers.hpp"

vector<string> split(const string &str) {
    istringstream iss(str);
    vector<string> res;
    string token;

    while (iss >> token)
        res.push_back(token);

    return res;
}

/**
 * Parses a string representation of a puzzle and returns a unique_ptr to a Puzzle object.
 * @param puzzleStr The string representation of the puzzle.
 * @return A unique_ptr to a Puzzle object.
 */
unique_ptr<Puzzle> parse(string puzzleStr) {
    unique_ptr<Puzzle> res;
    auto cells = split(puzzleStr);
    int size = sqrt(cells.size());
    res = make_unique<Puzzle>(size);
    
    for (size_t i = 0; i < cells.size(); i++)
        res->flattenStartState.push_back(stoi(cells[i]));

    return res;
}

/**
 * Creates a snail-shaped matrix of integers with the given size.
 * The snail starts at the top-left corner and spirals inward clockwise.
 * @param size The size of the matrix to create.
 * @return A 2D vector of integers representing the snail-shaped matrix.
 */
vector<vector<int>> createSnail(int size) {
    auto res = vector<vector<int>>(size, vector<int>(size, 0));
    int max = size * size;
    int val = 1; // The value to be inserted
    int rowStart = 0, rowEnd = size - 1;
    int colStart = 0, colEnd = size - 1;

    while (rowStart <= rowEnd and colStart <= colEnd) {
        for (int j = colStart; j <= colEnd; ++j)
            res[rowStart][j] = VAL(val, max);
        ++rowStart;

        for (int i = rowStart; i <= rowEnd; ++i)
            res[i][colEnd] = VAL(val, max);
        --colEnd;

        for (int j = colEnd; j >= colStart; --j)
            res[rowEnd][j] = VAL(val, max);
        --rowEnd;

        for (int i = rowEnd; i >= rowStart; --i)
            res[i][colStart] = VAL(val, max);
        ++colStart;
    }

    return res;
}
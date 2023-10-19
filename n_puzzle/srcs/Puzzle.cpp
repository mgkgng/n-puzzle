#include "Puzzle.hpp"

Puzzle::Puzzle() {}

Puzzle::Puzzle(int _size) {

    size = _size; // The size (N) of the puzzle.

    goalState = createSnail(size); // 2D vector representing the goal state of the puzzle.
    for (auto row : goalState)
        for (auto e : row)
            flattenGoalState.push_back(e); // 1D vector representing the flattened goal state of the puzzle.

    goalCoordinates = vector<pair<int, int>>(size * size); // 1D vector representing the coordinates of each tile in the goal state of the puzzle.
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int val = goalState[i][j];
            goalCoordinates[val] = {i, j};
        }
    }
}

Puzzle::~Puzzle() {}
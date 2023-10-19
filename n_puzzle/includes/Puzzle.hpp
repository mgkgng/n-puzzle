#pragma once

#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> createSnail(int size);

class Puzzle {
public:
    int size; // The size (N) of the puzzle.
    vector<vector<int>> goalState; // 2D vector representing the goal state of the puzzle.
    vector<int> flattenStartState; // 1D vector representing the flattened start state of the puzzle.
    vector<int> flattenGoalState; // 1D vector representing the flattened goal state of the puzzle.
    vector<pair<int, int>> goalCoordinates; // 1D vector representing the coordinates of each tile in the goal state of the puzzle.

    Puzzle();
    Puzzle(int _size);
    ~Puzzle();

};

extern unique_ptr<Puzzle> puzzle;
#pragma once

#include <vector>

#include "Puzzle.hpp"

extern unique_ptr<Puzzle> puzzle;

using namespace std;

bool isSolvable(vector<int>& start, const vector<int>& goal);
#pragma once

#include <cmath>
#include <vector>

#include "Puzzle.hpp"
#include "utilities.hpp"

extern unique_ptr<Puzzle> puzzle;

using namespace std;

int hFunction(int hChoice, const vector<int>& state);
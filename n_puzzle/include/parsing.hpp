#pragma once

#include <cmath>
#include <memory>
#include <random>
#include <vector>

#include "Puzzle.hpp"
#include "utilities.hpp"

extern unique_ptr<Puzzle> puzzle;

unique_ptr<Puzzle> parse(const int ac, string puzzleStr);
void printGrid(const vector<vector<int>>& grid);
vector<vector<int>> generate(int size);
vector<string> split(const string &str);
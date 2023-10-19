#pragma once

#include <chrono>
#include <cmath>
#include <limits>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <unordered_set>
#include <sstream>
#include <string>
#include <vector>

#include "Puzzle.hpp"
#include "Node.hpp"

// constants
#define VAL(x, max) ((x != max) ? x++ : 0) // The last number of the goal spiral should be 0
const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};
const char moves[4] = {'U', 'D', 'L', 'R'};

// algorithms.cpp
typedef int (*CostFunction)(const Node&, const vector<int>&, int hChoice);
int gCost(const Node& curr, const vector<int>& tmp, int hChoice);
int hCost(const Node& curr, const vector<int>& tmp, int hChoice);
int zeroCost(const Node& curr, const vector<int>& tmp, int hChoice);
vector<Node *> A_star(int hChoice, int &totalStatesVisited, int &maxStatesInMemory, CostFunction calculateG, CostFunction calculateH);
vector<Node *> IDA_star(int hChoice, int &totalStatesVisited, int &maxStatesInMemory);

// heuristic.cpp
int hFunction(int hChoice, const vector<int>& state);

// parsing.cpp
vector<string> split(const string &str);
unique_ptr<Puzzle> parse(string puzzleStr);

// solvability.cpp
bool isSolvable(vector<int>& start, const vector<int>& goal);

// utilities.cpp
int myAbs(int x);
struct VectorHash {
	size_t operator()(const vector<int>& vec) const;
};
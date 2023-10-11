#pragma once

#include <limits>
#include <queue>
#include <unordered_set>
#include <vector>

#include "Node.hpp"
#include "Puzzle.hpp"
#include "heuristic.hpp"
#include "utilities.hpp"

extern unique_ptr<Puzzle> puzzle;

typedef int (*CostFunction)(const Node&, const vector<int>&, int hChoice);
int gCost(const Node& curr, const vector<int>& tmp, int hChoice);
int hCost(const Node& curr, const vector<int>& tmp, int hChoice);
vector<Node *> A_star(int hChoice, int &totalStatesVisited, int &maxStatesInMemory, CostFunction calculateG, CostFunction calculateH);
vector<Node *> IDA_star(int hChoice, int &totalStatesVisited, int &maxStatesInMemory);
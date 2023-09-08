#pragma once

#include <cmath>
#include <map>

#include "puzzle.hpp"

using namespace std;

struct pState {
    vector<vector<int>> board;
    int cost, hFunction;

    bool operator==(const pState &other) const {
        return board == other.board;
    }
};

// IDA*
int IDAstar(pState &initialState, int hChoice, vector<char> &solution);

// Heuristic Functions
int Manhattan(const std::vector<std::vector<int>>& state);
int Hamming(const std::vector<std::vector<int>>& state);
int Euclidean(const std::vector<std::vector<int>>& state);
int hFunction(const pState &s, int hChoice);
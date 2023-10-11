#pragma once

#include <sstream>
#include <string>
#include <vector>

using namespace std;

#define VAL(x, max) ((x != max) ? x++ : 0) // The last number of the goal spiral should be 0

const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};
const char moves[4] = {'U', 'D', 'L', 'R'};

struct VectorHash {
    size_t operator()(const vector<int>& vec) const;
};

vector<string> split(const string &str);
int myAbs(int x);
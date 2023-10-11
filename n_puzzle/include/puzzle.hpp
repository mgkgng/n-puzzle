#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <queue>
#include <random>

using namespace std;

#define VAL(x, max) ((x != max) ? x++ : 0) // The last number of the goal spiral should be 0

const int dx[4] = {-1, 1, 0, 0}; // Possible moves in the x cartesian axis
const int dy[4] = {0, 0, -1, 1}; // Possible moves in the y cartesian axis
const char moves[4] = {'U', 'D', 'L', 'R'}; // Char representation of the moves

// parsing.cpp
vector<vector<int>> createSnail(int size);

class Puzzle {
    public:
        int size;
        vector<vector<int>> grid;
        vector<vector<int>> goalGrid;
        vector<int> initialGrid;
        vector<int> goalTest;
        vector<pair<int, int>> goalCoordinates;

    Puzzle() {}
    Puzzle(int _size) {
        size = _size;
        grid = vector<vector<int>>(_size, vector<int>(_size, 0));
        goalGrid = createSnail(size);

        for (auto row : goalGrid) {
            for (auto e : row)
                goalTest.push_back(e);
        }

        goalCoordinates = vector<pair<int, int>>(size * size);
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                int val = goalGrid[i][j];
                goalCoordinates[val] = {i, j};
            }
        }
    }

    ~Puzzle() {}

    friend ostream& operator<<(ostream& os, const Puzzle& p);
};

// Custom hash function for vectors
struct VectorHash {
    size_t operator()(const vector<int>& vec) const {
        hash<int> hasher;
        size_t seed = 0;
        for (int i : vec)
            seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

struct Node {
    vector<int> state;
    char move;
    int g;
    int f;
    Node* parent;

    Node(vector<int> _state, char _move) {
        state = _state;
        move = _move;
    }

    Node(vector<int> _state, char _move, int _g, int _f) {
        state = _state;
        move = _move;
        g = _g;
        f = _f;
        parent = nullptr;
    }
};

struct CompareNodes {
    bool operator()(const Node* left, const Node* right) const {
        return left->f > right->f;
    }
};

// algorithms.cpp
typedef int (*CostFunction)(const Node&, const vector<int>&, int hChoice);
int gCost(const Node& curr, const vector<int>& tmp, int hChoice);
int hCost(const Node& curr, const vector<int>& tmp, int hChoice);
vector<Node *> A_star(int hChoice, int &totalStatesVisited, int &maxStatesInMemory, CostFunction calculateG, CostFunction calculateH);
vector<Node *> IDA_star(int hChoice, int &totalStatesVisited, int &maxStatesInMemory);

// heuristic.cpp
int hFunction(int hChoice, const vector<int>& state);

// parsing.cpp
extern unique_ptr<Puzzle> puzzle;
unique_ptr<Puzzle> parse(const int ac, string puzzleStr);
void printGrid(const vector<vector<int>>& grid);
vector<vector<int>> generate(int size);

// solvable.cpp
bool isSolvable(vector<int>& start, const vector<int>& goal);

inline ostream& operator<<(ostream& os, const Puzzle& p) {
    os << "Initial Grid (" << p.size << "x" << p.size << "):"<< endl;
    for (const auto& row : p.grid) {
        for (const auto& cell : row)
            os << setw(4) << cell;
        os << endl;
    }
    return os;
}

inline vector<string> split(const string &str) {
    istringstream iss(str);
    vector<string> res;

    string token;
    while (iss >> token)
        res.push_back(token);
    return res;
}

inline int myAbs(int x) {
    return (x < 0) ? -x : x;
}
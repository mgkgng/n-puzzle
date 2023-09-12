#pragma once

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stack>
#include <unordered_set>
#include <vector>
#include <sstream> 
#include <string>
#include <cmath>
#include <map>

using namespace std;

#define VAL(x, max) ((x != max) ? x++ : 0) // The last number of the goal spiral should be 0

struct pState {
    vector<vector<int>> board;
    int cost, hFunction;

    bool operator==(const pState &other) const {
        return board == other.board;
    }
};

struct Node {
    vector<int> state;
    char move;

    // Node(vector<vector<int>> _state2D, char _move) {
    //     for (auto row : _state2D) {
    //         for (auto e : row) {
    //             state.push_back(e);
    //         }
    //     }
    //     move = _move;
    // }

    Node(vector<int> _state, char _move) {
        state = _state;
        move = _move;
    }
};


int hFunction(const pState &s, int hChoice);
int IDAstar(pState &initialState, int hChoice, vector<char> &solution);
vector<Node *> try_IDA(int hChoice);

class Puzzle {
    public:
        int size;
        vector<vector<int>> grid;
        vector<vector<int>> goalGrid;
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

    void printGrid(const vector<vector<int>>& grid) {
        cout << "Solution grid:" << endl;
        for (const auto& row : grid) {
            for (int val : row)
                cout << setw(4) << val;
            cout << endl;
        }
    }

    vector<vector<int>> createSnail(int size) {
        auto res = vector<vector<int>>(size, vector<int>(size, 0));
        int max = size * size;
        int val = 1; // The value to be inserted
        int rowStart = 0, rowEnd = size - 1;
        int colStart = 0, colEnd = size - 1;

        while (rowStart <= rowEnd && colStart <= colEnd) {
            for (int j = colStart; j <= colEnd; ++j)
                res[rowStart][j] = VAL(val, max);
            ++rowStart;

            for (int i = rowStart; i <= rowEnd; ++i)
                res[i][colEnd] = VAL(val, max);
            --colEnd;

            for (int j = colEnd; j >= colStart; --j)
                res[rowEnd][j] = VAL(val, max);
            --rowEnd;

            for (int i = rowEnd; i >= rowStart; --i)
                res[i][colStart] = VAL(val, max);
            ++colStart;
        }
        printGrid(res);
        return res;
    }

    friend ostream& operator<<(ostream& os, const Puzzle& p);
};

extern unique_ptr<Puzzle> puzzle;

unique_ptr<Puzzle> parse(const string &filename);

inline ostream& operator<<(ostream& os, const Puzzle& p) {
    os << "Puzzle size: " << p.size << endl;
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
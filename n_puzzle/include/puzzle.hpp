#pragma once

#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> createSnail(int size);

class Puzzle {
public:
    int size;
    vector<vector<int>> grid;
    vector<vector<int>> goalGrid;
    vector<int> initialGrid;
    vector<int> goalTest;
    vector<pair<int, int>> goalCoordinates;

    Puzzle();
    Puzzle(int _size);
    ~Puzzle();

    friend ostream& operator<<(ostream& os, const Puzzle& p);
};

#include "puzzle.hpp"

void printGrid(const vector<vector<int>>& grid) {
    for (const auto& row : grid) {
        for (int val : row)
            cout << setw(4) << val;
        cout << endl;
    }
    cout << endl;
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
    return res;
}

vector<vector<int>> generate(int size) {
    cout << "Generating 4 x 4 random puzzle..." << endl;
    random_device rd;
    mt19937 gen(rd());

    auto grid = createSnail(size);
    pair<int, int> zeroPos;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j] == 0) {
                zeroPos = make_pair(i, j);
                break;
            }
        }
    }
    
    vector<int> dirIndices = vector<int>();
    for (int iter = 0; iter < 3000; iter++) {
        dirIndices.clear();
        if (zeroPos.first > 0) dirIndices.push_back(0);
        if (zeroPos.first < size - 1) dirIndices.push_back(1);
        if (zeroPos.second > 0) dirIndices.push_back(2);
        if (zeroPos.second < size - 1) dirIndices.push_back(3); 

        std::uniform_int_distribution<> distrib(0, dirIndices.size() - 1);
        int dirIndex = dirIndices[distrib(gen)];
        swap(grid[zeroPos.first + dx[dirIndex]][zeroPos.second + dy[dirIndex]], grid[zeroPos.first][zeroPos.second]);
        zeroPos.first += dx[dirIndex];
        zeroPos.second += dy[dirIndex];
    }
    return grid;
}
#include "HeuristicFunctions.hpp"
#include <cmath>
#include <vector>
#include <map>

int Manhattan(const std::vector<std::vector<int>>& state, int N, const std::vector<std::vector<int>> &goalGrid) {
    int h = 0;

    std::map<int, std::vector<int>> helper;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            helper[state[i][j]] = {i, j};
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int val = goalGrid[i][j];
            if (val and val != state[i][j])
                h += abs(i - helper[val][0]) + abs(j - helper[val][1]);
        }
    }

    return h;
}

int Hamming(const std::vector<std::vector<int>>& state, int N, const std::vector<std::vector<int>> &goalGrid) {
    int h = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int val = state[i][j];
            if (val and val != goalGrid[i][j]) { // if val is not 0 and not in the right place
                h++;
            }
        }
    }
    return h;
}

int Euclidean(const std::vector<std::vector<int>>& state, int N, const std::vector<std::vector<int>> &goalGrid) {
    int h = 0;

    std::map<int, std::vector<int>> helper;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            helper[state[i][j]] = {i, j};
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int val = goalGrid[i][j];  
            if (val and val != state[i][j])
                h += sqrt(pow(i - helper[val][0], 2) + pow(j - helper[val][1], 2));
        }
    }

    return h;
}
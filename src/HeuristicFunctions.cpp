#include "HeuristicFunctions.hpp"
#include <cmath>

int Manhattan(const std::vector<std::vector<int>>& state, int N) {
    int h = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int val = state[i][j];
            if (val) {
                int tRow = (val - 1) / N;
                int tCol = (val - 1) % N;
                h += abs(i - tRow) + abs(j - tCol);
            }
        }
    }
    return h;
}

int Hamming(const std::vector<std::vector<int>>& state, int N) {
    int h = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int val = state[i][j];
            if (val and val != i * N + j + 1) { // if val is not 0 and not in the right place
                h++;
            }
        }
    }
    return h;
}

int Euclidean(const std::vector<std::vector<int>>& state, int N) {
    int h = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int val = state[i][j];
            if (val) {
                int tRow = (val - 1) / N;
                int tCol = (val - 1) % N;
                h += sqrt(pow(i - tRow, 2) + pow(j - tCol, 2));
            }
        }
    }
    return h;
}
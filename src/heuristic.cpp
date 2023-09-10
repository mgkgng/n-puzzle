#include "puzzle.hpp"

inline int myAbs(int x) {
    return (x < 0) ? -x : x;
}

int manhattan(const vector<vector<int>>& state, const vector<pair<int, int>>& goalCoordinates) {
    int h = 0;

    for (int i = 0; i < puzzle->size; ++i) {
        for (int j = 0; j < puzzle->size; ++j) {
            int val = state[i][j];
            if (val != 0) {
                int goalRow = goalCoordinates[val].first;
                int goalCol = goalCoordinates[val].second;
                h += myAbs(i - goalRow) + myAbs(j - goalCol);
            }
        }
    }

    return h;
}

int hamming(const vector<vector<int>>& state) {
    int h = 0;

    for (int i = 0; i < puzzle->size; ++i) {
        for (int j = 0; j < puzzle->size; ++j) {
            int val = state[i][j];
            if (val and val != puzzle->goalGrid[i][j]) // if val is not 0 and not in the right place
                h++;
        }
    }

    return h;
}

int euclidean(const vector<vector<int>>& state, const vector<pair<int, int>>& goalCoordinates) {
    int h = 0;

    for (int i = 0; i < puzzle->size; ++i) {
        for (int j = 0; j < puzzle->size; ++j) {
            int val = state[i][j];
            if (val != 0) {
                int goalRow = goalCoordinates[val].first;
                int goalCol = goalCoordinates[val].second;
                double distance = sqrt(static_cast<double>((i - goalRow) * (i - goalRow) + (j - goalCol) * (j - goalCol)));
                h += static_cast<int>(distance);
            }
        }
    }

    return h;
}

int hFunction(const pState &s, int hChoice) {
    switch (hChoice) {
        case 1:
            return manhattan(s.board, puzzle->goalCoordinates);
        case 2:
            return hamming(s.board);
        case 3:
            return euclidean(s.board, puzzle->goalCoordinates);
        default:
            return manhattan(s.board, puzzle->goalCoordinates);
    }
}
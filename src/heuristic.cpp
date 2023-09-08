#include "npuzzle.hpp"

int Manhattan(const vector<vector<int>>& state) {
    int h = 0;

    map<int, vector<int>> helper;
    for (int i = 0; i < puzzle->size; i++) {
        for (int j = 0; j < puzzle->size; j++) {
            helper[state[i][j]] = {i, j};
        }
    }

    for (int i = 0; i < puzzle->size; i++) {
        for (int j = 0; j < puzzle->size; j++) {
            int val = puzzle->goalGrid[i][j];
            if (val and val != state[i][j])
                h += abs(i - helper[val][0]) + abs(j - helper[val][1]);
        }
    }

    return h;
}

int Hamming(const vector<vector<int>>& state) {
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

int Euclidean(const vector<vector<int>>& state) {
    int h = 0;

    map<int, vector<int>> helper;
    for (int i = 0; i < puzzle->size; i++) {
        for (int j = 0; j < puzzle->size; j++)
            helper[state[i][j]] = {i, j};
    }

    for (int i = 0; i < puzzle->size; i++) {
        for (int j = 0; j < puzzle->size; j++) {
            int val = puzzle->goalGrid[i][j];  
            if (val and val != state[i][j])
                h += sqrt(pow(i - helper[val][0], 2) + pow(j - helper[val][1], 2));
        }
    }

    return h;
}

int hFunction(const pState &s, int hChoice) {
    switch (hChoice) {
        case 1:
            return Manhattan(s.board);
        case 2:
            return Hamming(s.board);
        case 3:
            return Euclidean(s.board);
        default:
            return Manhattan(s.board);
    }
}
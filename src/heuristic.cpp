#include "puzzle.hpp"

int manhattan(const vector<int>& state) {
    int h = 0;
    for (int i = 0; i < puzzle->size * puzzle->size; ++i) {
        if (state[i] != 0 and state[i] != puzzle->goalTest[i]) {
            auto val = puzzle->goalCoordinates[state[i]];
            h += myAbs(i / puzzle->size - val.first) + myAbs(i % puzzle->size - val.second);
        }
    }
    return h;
}

int hamming(const vector<int>& state) {
    int h = 0;
    for (int i = 0; i < puzzle->size * puzzle->size; ++i)
        if (state[i] != 0 and state[i] != puzzle->goalTest[i])
            h++;
    return h;
}

int linearConflict(const vector<int>& state) {
    int h = 0;
    for (int i = 0; i < (puzzle->size * puzzle->size) - 1; ++i) {
        int val = state[i];
        if (val != 0) {
            int goalRow = puzzle->goalCoordinates[val].first;
            int goalCol = puzzle->goalCoordinates[val].second;
            for (int k = i + 1; k < (puzzle->size * puzzle->size) - 1; ++k) {
                int nextVal = state[k];
                if (nextVal != 0 and (
                (puzzle->goalCoordinates[nextVal].first == goalRow and (k / 3 == puzzle->goalCoordinates[nextVal].first) and k - i == 1) or
                (puzzle->goalCoordinates[nextVal].second == goalCol and (k % 3 == puzzle->goalCoordinates[nextVal].second) and k - i == 3)
                )) {
                    if (val > nextVal) {
                        h++;
                    }
                }
            }
        }
    }
    return 2 * h; 
}

int hFunction(int hChoice, const vector<int>& state) {
    switch (hChoice) {
        case 1:
            return manhattan(state);
        case 2:
            return hamming(state);
        case 3:
            return linearConflict(state);
        default:
            return -1;
    }
}
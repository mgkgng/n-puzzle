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

//int linearConflict(const vector<int>& state)

int hFunction(int hChoice, const vector<int>& state) {
    switch (hChoice) {
        case 1:
            return manhattan(state);
        case 2:
            return hamming(state);
        //case 3:
        //    return linearConflict(state);
        default:
            return -1;
    }
}
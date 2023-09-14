#include "puzzle.hpp"

int manhattan(const vector<int>& state) {
    int h = 0;
    for (int i = 0; i < (puzzle->size * puzzle->size) - 1; ++i) {
        if (state[i] != 0 and state[i] != puzzle->goalTest[i]) {
            auto val = puzzle->goalCoordinates[state[i]];
            h += myAbs(i / puzzle->size - val.first) + myAbs(i % puzzle->size - val.second);
        }
    }
    return h;
}

int hamming(const vector<int>& state) {
    int h = 0;
    for (int i = 0; i < (puzzle->size * puzzle->size) - 1; ++i)
        if (state[i] != 0 and state[i] != puzzle->goalTest[i])
            h++;
    return h;
}

//int linearConflict(const vector<vector<int>>& state) {
//    int h = 0;
//    for (int i = 0; i < puzzle->size; ++i) {
//        for (int j = 0; j < puzzle->size; ++j) {
//            int val = state[i][j];
//            if (val != 0) {
//                int goalRow = puzzle->goalCoordinates[val].first;
//                int goalCol = puzzle->goalCoordinates[val].second;
//                if (i != goalRow or j != goalCol) {
//                    for (int k = j + 1; k < puzzle->size; ++k) {
//                        int nextVal = state[i][k];
//                        if (nextVal != 0 and puzzle->goalCoordinates[nextVal].first == goalRow
//                            and i == puzzle->goalCoordinates[nextVal].first) {
//                            if (val > nextVal) {
//                                h++;
//                            }
//                        }
//                    }
//                    for (int k = i + 1; k < puzzle->size; ++k) {
//                        int nextVal = state[k][j];
//                        if (nextVal != 0 and puzzle->goalCoordinates[nextVal].second == goalCol
//                            and j == puzzle->goalCoordinates[nextVal].second) {
//                            if (val > nextVal) {
//                                h++;
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//    return h;
//}

//int manhattanLinearConflict(const vector<vector<int>>& state2D, const vector<int>& state1D) {
//    int manhattanDistance = manhattan(state1D);
//    int linearConflictValue = linearConflict(state2D);

//    return manhattanDistance + 2 * linearConflictValue;
//}

int hFunction(int hChoice, const vector<int>& state) {
    switch (hChoice) {
        case 1:
            return manhattan(state);
        case 2:
            return hamming(state);
        default:
            return -1;
        //case 3
        //    return linearConflict();
        //case 4:
        //    return combinedHeuristic();
    }
}
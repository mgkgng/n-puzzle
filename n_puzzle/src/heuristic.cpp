#include "puzzle.hpp"

/**
 * Calculates the Manhattan distance heuristic for the given state with respect to the goal state.
 * The Manhattan distance is the sum of the horizontal and vertical distances between the current state and the goal state.
 * @param state The state of the puzzle as a vector of integers.
 * @return The Manhattan distance heuristic value.
 */
static int manhattan(const vector<int>& state) {
    int h = 0;
    for (int i = 0; i < puzzle->size * puzzle->size; ++i) {
        if (state[i] != 0 and state[i] != puzzle->goalTest[i]) {
            auto val = puzzle->goalCoordinates[state[i]];
            h += myAbs(i / puzzle->size - val.first) + myAbs(i % puzzle->size - val.second);
        }
    }
    return h;
}

/**
 * Calculates the Hamming distance heuristic for the given state.
 * The Hamming distance is the number of misplaced tiles in the state with respect to the goal state.
 * @param state The state to calculate the heuristic for.
 * @return The Hamming distance heuristic value.
 */
static int hamming(const vector<int>& state) {
    int h = 0;
    for (int i = 0; i < puzzle->size * puzzle->size; ++i)
        if (state[i] != 0 and state[i] != puzzle->goalTest[i])
            h++;
    return h;
}

/**
 * Calculates the Euclidean distance for the given state with respect to the goal state.
 * The Euclidean distance is the straight-line distance between the current state and the goal state.
 * @param state The current state of the puzzle.
 * @return The Euclidean distance between the current state and the goal state.
 */
static int euclidian(const vector<int>& state) {
    int h = 0;
    for (int i = 0; i < puzzle->size * puzzle->size; ++i) {
        if (state[i] != 0 and state[i] != puzzle->goalTest[i]) {
            auto val = puzzle->goalCoordinates[state[i]];
            h += sqrt(pow(i / puzzle->size - val.first, 2) + pow(i % puzzle->size - val.second, 2));
        }
    }
    return h;
}

/**
 * Calculates the linear conflict heuristic for the given state.
 * Linear conflict is the number of conflicts between tiles in their goal positions
 * that are in the same row or column and require the same row or column movement to resolve.
 * @param state The state to calculate the heuristic for.
 * @return The linear conflict heuristic value for the given state.
 */
static int linearConflict(const vector<int>& state) {
    int h = 0;
    for (int i = 0; i < puzzle->size * puzzle->size; ++i) {
        int val = state[i];
        if (val != 0) {
            int goalRow = puzzle->goalCoordinates[val].first;
            int goalCol = puzzle->goalCoordinates[val].second;
            for (int k = i + 1; k < puzzle->size * puzzle->size; ++k) {
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
    return h; 
}

/**
 * Calculates the Manhattan distance heuristic with linear conflict for the given state.
 * @param state The state of the puzzle as a vector of integers.
 * @return The calculated heuristic value as an integer.
 */
int manhattanLinearConflict(const vector<int>& state) {
    return manhattan(state) + 2 * linearConflict(state);
}

/**
 * Calculates the heuristic value of a given state based on the selected heuristic function.
 * @param hChoice An integer representing the selected heuristic function.
 * @param state A vector of integers representing the current state of the puzzle.
 * @return An integer representing the heuristic value of the given state.
 */
int hFunction(int hChoice, const vector<int>& state) {
    switch (hChoice) {
        case 1:
            return manhattan(state);
        case 2:
            return hamming(state);
        case 3:
            return euclidian(state);
        case 4:
            return manhattanLinearConflict(state);
        default:
            return -1;
    }
}
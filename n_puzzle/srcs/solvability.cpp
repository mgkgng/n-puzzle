#include "headers.hpp"

/**
 * Calculates the inversion count based on the given start and goal states.
 * An inversion is when an element start[i] appears before an element start[j] in the start state but after it in the goal state
 * @param start The start state of the N-Puzzle represented as a 1D vector of integers.
 * @param goal The goal state of the N-Puzzle represented as a 1D vector of integers.
 * @return The inversion count between the two vectors.
 */
static int invCount(const vector<int>& start, const vector<int>& goal) {
    int count = 0;

    for (size_t i = 0; i < start.size(); ++i) {
        for (size_t j = i; j < start.size(); ++j) {
            auto start_i_iter = std::find(goal.begin(), goal.end(), start[i]);
            auto start_j_iter = std::find(goal.begin(), goal.end(), start[j]);
            if (start_i_iter > start_j_iter)
                count += 1;
        }
    }

    return count;
}

/**
 * Finds the position of the zero tile (empty space) in a given vector of integers
 * @param state The 1D vector of integers to search for the zero tile.
 * @return A pair of integers representing the row and column indices of the zero tile.
 */
static pair<int, int> findZeroTile(const vector<int>& state) {
    for (size_t i = 0; i < state.size(); ++i)
        if (state[i] == 0)
            return {i / puzzle->size, i % puzzle->size};

    return {-1, -1};
}

/**
 * Calculates the Manhattan distance of the zero tile based on the given start and goal states.
 * @param start The start state of the N-Puzzle represented as a 1D vector of integers.
 * @param goal The goal state of the N-Puzzle represented as a 1D vector of integers.
 * @return The Manhattan distance between the two 1D vectors.
 */
static int manhattan(const vector<int>& start, const vector<int>& goal) {
    pair<int, int> s, g;
    s = findZeroTile(start);
    g = findZeroTile(goal);

    return abs(g.first - s.first) + abs(g.second - s.second);
}

/**
 * Determines whether a given N-Puzzle start state is solvable or not based on the goal state.
 * @param start The start state of the N-Puzzle represented as a 1D vector of integers.
 * @param goal The goal state of the N-Puzzle represented as a 1D vector of integers.
 * @return True if the start state is solvable, false otherwise.
 */
bool isSolvable(vector<int>& start, const vector<int>& goal) {
    int i = invCount(start, goal);
    int m = manhattan(start, goal);

    if (i % 2 == m % 2)
        return true;

    return false;
}
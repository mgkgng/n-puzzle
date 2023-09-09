#include "npuzzle.hpp"

static bool move(int dx, int dy, pState &s) {
    int x, y;
    for (x = 0; x < puzzle->size; ++x) {
        for (y = 0; y < puzzle->size; ++y)
            if (s.board[x][y] == 0)
                break ;
        if (y < puzzle->size)
            break ;
    }

    int newX = x + dx;
    int newY = y + dy;

    if (newX >= 0 and newX < puzzle->size and newY >= 0 and newY < puzzle->size) {
        swap(s.board[x][y], s.board[newX][newY]);
        return true;
    } else
        return false;
}

static bool isGoal(const pState &s) {
    return s.board == puzzle->goalGrid;
}

static int search(pState &s, int g, int threshold, int hChoice, vector<char> &solution) {
    int f = g + hFunction(s, hChoice);

    if (f > threshold) // Current branch should be pruned because it exceeds the threshold
        return f;

    if (isGoal(s)) // Solution found
        return -1; 

    int minNextThreshold = numeric_limits<int>::max();

    int dx[] = {-1, 1, 0, 0}; // Possible moves in the x cartesian axis
    int dy[] = {0, 0, -1, 1}; // Possible moves in the y cartesian axis
    char moves[] = {'U', 'D', 'L', 'R'}; // Char representation of the moves

    for (int i = 0; i < 4; ++i) { // For each possible move
        if (move(dx[i], dy[i], s)) { // If the move is valid apply it to the state
            int newThreshold = search(s, g + 1, threshold, hChoice, solution); // Recursively call the search function with the new state
            if (newThreshold == -1) { // Solution found in the subtree
                solution.push_back(moves[i]); // Add the move to the solution vector
                return -1;
            }
            if (newThreshold < minNextThreshold) // Update the minimum threshold
                minNextThreshold = newThreshold;
            move(-dx[i], -dy[i], s); // Undo the move to restore the state to i
        }
    }

    return minNextThreshold;
}

int IDAstar(pState &initialState, int hChoice, vector<char> &solution) {
    int threshold = hFunction(initialState, hChoice); // Initial threshold
    while (true) {
        int nextThreshold = search(initialState, 0, threshold, hChoice, solution); // Search for a solution with the current threshold
        if (nextThreshold == -1) // Solution found
            return threshold;
        if (nextThreshold == numeric_limits<int>::max()) // No solution found
            return -1;
        threshold = nextThreshold; // Update the threshold
    }
}
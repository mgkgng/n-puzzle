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

// bound = threshold
static int search(pState &s, int g, int bound, int hChoice, vector<char> &solution) {
    int f = g + hFunction(s, hChoice);

    if (f > bound)
        return f;

    if (isGoal(s))
        return -1;

    int minNextBound = numeric_limits<int>::max();

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    char moves[] = {'U', 'D', 'L', 'R'};

    for (int i = 0; i < 4; ++i) {
        if (move(dx[i], dy[i], s)) {
            int newBound = search(s, g + 1, bound, hChoice, solution);
            if (newBound == -1) {
                solution.push_back(moves[i]); // Add the move to the solution
                return -1; // Solution found in the subtree
            }
            if (newBound < minNextBound)
                minNextBound = newBound;
            move(-dx[i], -dy[i], s); // Undo the move
        }
    }

    return minNextBound;
}

int IDAstar(pState &initialState, int hChoice, vector<char> &solution) {
    int bound = hFunction(initialState, hChoice);
    while (true) {
        int nextBound = search(initialState, 0, bound, hChoice, solution);
        if (nextBound == -1)
            return bound; // Solution found
        if (nextBound == numeric_limits<int>::max())
            return -1; // No solution found
        bound = nextBound;
    }
}

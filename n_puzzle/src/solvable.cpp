#include "puzzle.hpp"

int invCount(const vector<int>& state) {
    int count = 0;

    for (int i = 0; i < puzzle->size * puzzle->size; ++i)
        for (int j = i + 1; j < puzzle->size * puzzle->size; ++j)
            if (state[i] && state[j] && state[i] > state[j])
                count++;
    return count;
}

int findXPos(const vector<vector<int>> &curr) {
    for (int i = puzzle->size - 1; i >= 0; i--)
        for (int j = puzzle->size - 1; j >= 0; j--)
            if (curr[i][j] == 0)
                return puzzle->size - i;
    return -1;
}

bool isSolvable(const vector<int>& state, const vector<vector<int>> &curr) {
    int inv = invCount(state);

    if (puzzle->size % 2 == 1)
        return (inv % 2);
    else {
        int zeroPos = findXPos(curr);
        if (zeroPos % 2 == 1)
            return not (inv % 2);
        else
            return (inv % 2);
    }
}
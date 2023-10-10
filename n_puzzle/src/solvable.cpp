#include "puzzle.hpp"

int invCount(vector<int>& start, const vector<int>& goal) {
    int count = 0;
    int idx;
    for (size_t i = 0; i < start.size(); ++i) {
        if (start[i] != goal[i]) {
            idx = find(start.begin(), start.end(), goal[i]) - start.begin();
            swap(start[i], start[idx]);
            count++;
        }
    }
    return count;
}

pair<int, int> findZeroTile(const vector<int>& matrix) {
    for (size_t i = 0; i < matrix.size(); ++i)
        if (matrix[i] == 0)
            return {i / puzzle->size, i % puzzle->size};
    return {-1, -1};
}

int manhattan(const vector<int>& start, const vector<int>& goal) {
    pair<int, int> s, g;
    s = findZeroTile(start);
    g = findZeroTile(goal);
    return abs(g.first - s.first) + abs(g.second - s.second);
}

bool isSolvable(vector<int>& start, const vector<int>& goal) {
    int i = invCount(start, goal);
    int m = manhattan(start, goal);
    if (i % 2 == m % 2)
        return true;
    return false;
}
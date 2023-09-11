#include "puzzle.hpp"
#include <math.h>

// a mon avis, a faire : recursive -> iterative

int dx[4] = {-1, 1, 0, 0}; // Possible moves in the x cartesian axis
int dy[4] = {0, 0, -1, 1}; // Possible moves in the y cartesian axis
char moves[4] = {'U', 'D', 'L', 'R'}; // Char representation of the moves

int re_manhattan(const vector<vector<int>>& state) {
    int h = 0;

    for (int i = 0; i < puzzle->size; ++i) {
        for (int j = 0; j < puzzle->size; ++j) {
            int val = state[i][j];
            if (val != 0) {
                int goalRow = puzzle->goalCoordinates[val].first;
                int goalCol = puzzle->goalCoordinates[val].second;
                h += abs(i - goalRow) + abs(j - goalCol);
            }
        }
    }
    return h;
}

pair<int, int> findZero(const vector<vector<int>> &curr) {
    for (int i = 0; i < puzzle->size; i++) {
        for (int j = 0; j < puzzle->size; j++) {
            if (curr[i][j] == 0)
                return make_pair(i, j);
        }
    }
    return make_pair(-1, -1);
}

bool checkDouble(vector<Node *> &path, const string &hash) {
    for (auto p : path) {
        if (p->hash == hash)
            return true;
    }
    return false;
}

string hashState(const vector<vector<int>> &state) {
    string hash;
    for (const auto &row : state) {
        for (int val : row) {
            hash += to_string(val) + ',';
        }
    }
    return hash;
}

int re_search(vector<Node *> &path, int g, int threshold, int hChoice) {
    Node *curr = path[path.size() - 1];

    float f = g + re_manhattan(curr->state);
    if (f > threshold)
        return f;

    if (curr->state == puzzle->goalGrid) // isGoal
        return -1;

    int min = numeric_limits<int>::max();

    for (int i = 0; i < 4; ++i) { // For each possible move
        const auto zeroPos = findZero(curr->state);
        const auto newPos = make_pair(zeroPos.first + dx[i], zeroPos.second + dy[i]);

        if (!(newPos.first >= 0 && newPos.first < puzzle->size && newPos.second >= 0 && newPos.second < puzzle->size)) // check valid move
            continue;

        auto tmp = curr->state;
        swap(tmp[zeroPos.first][zeroPos.second], tmp[newPos.first][newPos.second]); // move
        const string tmpHash = hashState(tmp); // hashing (optimization)
        if (checkDouble(path, tmpHash) == true) // if curr node in path (already visited state ?)
            continue;

        path.push_back(new Node(tmp, moves[i], hashState(tmp)));
        int t = re_search(path, g + 1, threshold, hChoice); 
        if (t == -1) // path found
            return -1;
        if (t < min)
            min = t;
        path.pop_back();
    }
    return min;
}

vector<Node *> try_IDA(int hChoice) {
    Node *root = new Node(puzzle->grid, 'O', hashState(puzzle->grid));
    int threshold = re_manhattan(puzzle->grid);
    vector<Node *> path = { root };
    while (true) {
        int t = re_search(path, 0, threshold, hChoice);
        if (t == -1) {
            return path;
        } else if (t == numeric_limits<int>::max())
            return vector<Node *>();
        threshold = t;
    } 
    return path;
}

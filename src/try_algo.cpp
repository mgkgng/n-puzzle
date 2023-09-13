#include "puzzle.hpp"
#include <math.h>

// a mon avis, a faire : recursive -> iterative

int dx[4] = {-1, 1, 0, 0}; // Possible moves in the x cartesian axis
int dy[4] = {0, 0, -1, 1}; // Possible moves in the y cartesian axis
char moves[4] = {'U', 'D', 'L', 'R'}; // Char representation of the moves

int re_manhattan(const vector<int>& state) {
    int h = 0;

    for (int i = 0; i < puzzle->size * puzzle->size - 1; ++i) {
        if (state[i] && state[i] != puzzle->goalTest[i]) {
            auto val = puzzle->goalCoordinates[state[i]];
            h += abs(i / puzzle->size - val.first) + abs(i % puzzle->size - val.second);
        }
    }

    return h;
}

int invCount(const vector<int>& state) {
    int count = 0;

    for (int i = 0; i < puzzle->size * puzzle->size; ++i) {
        for (int j = i + 1; j < puzzle->size * puzzle->size; ++j) {
            if (state[i] && state[j] && state[i] > state[j])
                count++;
        }
    }
    return count;
}

int findZero(const vector<int> &curr) {
    for (int i = 0; i < puzzle->size * puzzle->size; i++) {
        if (curr[i] == 0)
            return i;
    }
    return -1;
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

bool checkDouble(vector<Node *> &path, const vector<int> &curr) {
    for (auto p : path) {
        if (p->state == curr)
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

int after_move(int pos, int dx, int dy) {
    int row = pos / puzzle->size;
    int col = pos % puzzle->size;
    if (!(row + dx >= 0 && row + dx < puzzle->size && col + dy >= 0 && col + dy < puzzle->size))
        return -1;
    return pos + dx * puzzle->size + dy;
}

int re_search(vector<Node *> &path, int g, int threshold, int hChoice) {
    Node *curr = path[path.size() - 1];

    
    float f = g + re_manhattan(curr->state);

    if (f > threshold)
        return f;

    if (curr->state == puzzle->goalTest) // isGoal
        return -1;

    int min = numeric_limits<int>::max();

    for (int i = 0; i < 4; ++i) { // For each possible move
        const auto zeroPos = findZero(curr->state);
        const auto newPos = after_move(zeroPos, dx[i], dy[i]);

        if (newPos < 0) { // check valid move
            continue;
        }
        auto tmp = curr->state;
        
        tmp[zeroPos] = tmp[newPos];
        tmp[newPos] = 0;
        // const string tmpHash = hashState(tmp); // hashing (optimization)
        if (checkDouble(path, tmp) == true) // if curr node in path (already visited state ?)
            continue;

        path.push_back(new Node(tmp, moves[i]));
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
    vector<int> hello;

    for (auto row : puzzle->grid) {
        for (auto e : row)
            hello.push_back(e);
    }
    Node *root = new Node(hello, 'O');
    int threshold = re_manhattan(puzzle->goalTest);
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

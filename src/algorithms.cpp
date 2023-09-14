#include "puzzle.hpp"

int dx[4] = {-1, 1, 0, 0}; // Possible moves in the x cartesian axis
int dy[4] = {0, 0, -1, 1}; // Possible moves in the y cartesian axis
char moves[4] = {'U', 'D', 'L', 'R'}; // Char representation of the moves

int findZero(const vector<int> &curr) {
    for (int i = 0; i < puzzle->size * puzzle->size; i++)
        if (curr[i] == 0)
            return i;
    return -1;
}

bool checkDouble(vector<Node *> &path, const vector<int> &curr) {
    for (auto p : path)
        if (p->state == curr)
            return true;
    return false;
}

string hashState(const vector<vector<int>> &state) {
    string hash;
    for (const auto &row : state)
        for (int val : row)
            hash += to_string(val) + ',';
    return hash;
}

int after_move(int pos, int dx, int dy) {
    int row = pos / puzzle->size;
    int col = pos % puzzle->size;
    if (not (row + dx >= 0 && row + dx < puzzle->size && col + dy >= 0 && col + dy < puzzle->size))
        return -1;
    return pos + dx * puzzle->size + dy;
}

vector<Node *> A_star(int hChoice) {
    vector<int> hello;

    for (auto row : puzzle->grid)
        for (auto e : row)
            hello.push_back(e);

    Node* root = new Node(hello, 'O', 0, 0);
    priority_queue<Node*, vector<Node*>, CompareNodes> pq;

    pq.push(root);
    vector<Node*> path;

    while (not pq.empty()) {
        Node* curr = pq.top();
        pq.pop();

        if (curr->state == puzzle->goalTest) {
            path.clear();
            while (curr != nullptr) {
                path.push_back(curr);
                curr = curr->parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 4; ++i) {
            const auto zeroPos = findZero(curr->state);
            const auto newPos = after_move(zeroPos, dx[i], dy[i]);

            if (newPos < 0) continue;
            auto tmp = curr->state;
            tmp[zeroPos] = tmp[newPos];
            tmp[newPos] = 0;
            
            if (checkDouble(path, tmp) == true) continue;

            int h = hFunction(hChoice, curr->state);
            int g = curr->g + 1;
            int f = g + h;

            Node* newNode = new Node(tmp, moves[i], g, f);
            newNode->parent = curr;

            pq.push(newNode);
        }
    }

    return vector<Node*>();
}

int search(vector<Node *> &path, int g, int threshold, int hChoice) {
    Node *curr = path[path.size() - 1];
    float f = g + hFunction(hChoice, curr->state);

    if (f > threshold)
        return f;

    if (curr->state == puzzle->goalTest) // isGoal
        return -1;

    int min = numeric_limits<int>::max();

    for (int i = 0; i < 4; ++i) { // For each possible move
        const auto zeroPos = findZero(curr->state);
        const auto newPos = after_move(zeroPos, dx[i], dy[i]);

        if (newPos < 0) continue;// check valid move

        auto tmp = curr->state;
        
        tmp[zeroPos] = tmp[newPos];
        tmp[newPos] = 0;
        // const string tmpHash = hashState(tmp); // hashing (optimization)
        if (checkDouble(path, tmp) == true) continue; // if curr node in path (already visited state ?)

        path.push_back(new Node(tmp, moves[i]));
        int t = search(path, g + 1, threshold, hChoice); 
        if (t == -1) // path found
            return -1;
        if (t < min)
            min = t;
        path.pop_back();
    }
    return min;
}

vector<Node *> IDA_star(int hChoice) {
    vector<int> hello;

    for (auto row : puzzle->grid)
        for (auto e : row)
            hello.push_back(e);

    Node *root = new Node(hello, 'O');
    int threshold = hFunction(hChoice, root->state);
    vector<Node *> path = { root };

    while (true) {
        int t = search(path, 0, threshold, hChoice);
        if (t == -1)
            return path;
        else if (t == numeric_limits<int>::max())
            return vector<Node *>();
        threshold = t;
    } 
    return path;
}
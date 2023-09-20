#include "puzzle.hpp"

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

int afterMove(int pos, int dx, int dy) {
    int row = pos / puzzle->size;
    int col = pos % puzzle->size;
    if (not (row + dx >= 0 && row + dx < puzzle->size && col + dy >= 0 && col + dy < puzzle->size))
        return -1;
    return pos + dx * puzzle->size + dy;
}

vector<Node *> A_star(int hChoice, int &totalStatesVisited, int &maxStatesInMemory) {
    vector<int> hello;
    for (auto row : puzzle->grid)
        for (auto e : row)
            hello.push_back(e);

    Node* root = new Node(hello, 'S', 0, 0);
    priority_queue<Node*, vector<Node*>, CompareNodes> pq; // Node with the lowest f value will be at the top of the queue thanks to the CompareNodes struct
    pq.push(root);

    vector<Node*> path; // Store the path from the root node to the current node

    unordered_set<vector<int>, VectorHash> visitedStates; // Store the visited states to avoid revisiting previously explored states

    while (not pq.empty()) {
        int currentStatesInMemory = pq.size();
        if (currentStatesInMemory > maxStatesInMemory)
            maxStatesInMemory = currentStatesInMemory;
        totalStatesVisited++;
        Node* curr = pq.top();
        pq.pop();

        if (visitedStates.find(curr->state) != visitedStates.end()) continue; // Skip this state if it has been visited
        visitedStates.insert(curr->state); // Mark the current state as visited

        if (curr->state == puzzle->goalTest) {
            path.clear();
            while (curr != nullptr) { // Reconstruction of the solution path by following the parent pointers from the current node to the root node (nullptr)
                path.push_back(curr);
                curr = curr->parent;
            }
            reverse(path.begin(), path.end()); // Reverse the path to get the correct order of the solution moves
            return path;
        }

        for (int i = 0; i < 4; ++i) { // Generate the children of the current node by swapping the blank tile with the four possible moves
            const auto zeroPos = findZero(curr->state);
            const auto newPos = afterMove(zeroPos, dx[i], dy[i]);

            if (newPos < 0) continue; // Skip the move if the new position of the blank tile is out of the grid bounds
            auto tmp = curr->state; // If the move is valid create a copy of the current state
            tmp[zeroPos] = tmp[newPos]; // Apply the move of the blank tile to the copy
            tmp[newPos] = 0; // Set the new position of the blank tile to 0

            if (visitedStates.find(tmp) != visitedStates.end()) continue; // Check if the new state has already been visited to avoid revisiting previously explored states

            int h = hFunction(hChoice, tmp);
            int g = curr->g + 1;
            int f = g + h; // Compute the f value of the new state

            Node* newNode = new Node(tmp, moves[i], g, f); // New node with the new state and the corresponding f value
            newNode->parent = curr; // Set the parent pointer of the new node to the current node

            pq.push(newNode);
        }
    }

    cout << "Total states visited: " << totalStatesVisited << endl;

    return vector<Node*>(); // Return empty vector if no solution is found
}

int search(vector<Node *> &path, int g, int threshold, int hChoice, int &totalStatesVisited, int &maxStatesInMemory) {
    Node *curr = path[path.size() - 1];
    float f = g + hFunction(hChoice, curr->state);

    if (f > threshold)
        return f;

    if (curr->state == puzzle->goalTest)
        return -1;

    int min = numeric_limits<int>::max();

    for (int i = 0; i < 4; ++i) { // Generate the children of the current node by swapping the blank tile with the four possible moves
        const auto zeroPos = findZero(curr->state);
        const auto newPos = afterMove(zeroPos, dx[i], dy[i]);

        if (newPos < 0) continue;

        auto tmp = curr->state;
        
        tmp[zeroPos] = tmp[newPos];
        tmp[newPos] = 0;
        if (checkDouble(path, tmp) == true) continue; // Check if the new state has already been visited to avoid revisiting previously explored states

        path.push_back(new Node(tmp, moves[i]));
        totalStatesVisited++;
        int currentStatesInMemory = path.size();
        if (currentStatesInMemory > maxStatesInMemory)
            maxStatesInMemory = currentStatesInMemory;

        int t = search(path, g + 1, threshold, hChoice, totalStatesVisited, maxStatesInMemory); 
        if (t == -1) // Solution found
            return -1;
        if (t < min)
            min = t;
        path.pop_back();
    }


    return min;
}

vector<Node *> IDA_star(int hChoice, int &totalStatesVisited, int &maxStatesInMemory) {
    vector<int> hello;

    for (auto row : puzzle->grid)
        for (auto e : row)
            hello.push_back(e);

    Node *root = new Node(hello, 'S');
    int threshold = hFunction(hChoice, root->state);
    vector<Node *> path = { root };

    while (true) {
        int t = search(path, 0, threshold, hChoice, totalStatesVisited, maxStatesInMemory);
        if (t == -1)
            return path;
        else if (t == numeric_limits<int>::max())
            return vector<Node *>();
        threshold = t;
    } 
    return path;
}
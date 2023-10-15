#include "headers.hpp"

/**
 * Finds the index of the zero element in a given vecto of integers.
 * @param curr The vector to search for the zero element.
 * @return The index of the zero element in the vector.
 */
static int findZero(const vector<int> &curr) {
    for (int i = 0; i < puzzle->size * puzzle->size; i++)
        if (curr[i] == 0)
            return i;
    return -1;
}

/**
 * Calculates the new position of a tile after a move is made.
 * @param pos The current position of the tile.
 * @param dx The horizontal distance to move the tile.
 * @param dy The vertical distance to move the tile.
 * @return The new position of the tile.
 */
static int afterMove(int pos, int dx, int dy) {
    int row = pos / puzzle->size;
    int col = pos % puzzle->size;
    if (not (row + dx >= 0 && row + dx < puzzle->size && col + dy >= 0 && col + dy < puzzle->size))
        return -1;
    return pos + dx * puzzle->size + dy;
}

/**
 * Calculates the cost of the path from the start node to the current node.
 * @param curr The current node.
 * @param tmp The current state of the puzzle.
 * @param hChoice The heuristic function to use.
 * @return The cost of the path from the start node to the current node.
 */
int gCost(const Node& curr, const vector<int>& tmp, int hChoice) {
    return curr.g + 1;
}

/**
 * Calculates the heuristic cost of a given node based on the chosen heuristic function.
 * @param curr The current node to calculate the heuristic cost for.
 * @param tmp The goal state of the puzzle.
 * @param hChoice The chosen heuristic function.
 * @return The heuristic cost of the current node.
 */
int hCost(const Node& curr, const vector<int>& tmp, int hChoice) {
    return hFunction(hChoice, tmp);
}

/**
 * Runs the A* search algorithm on the N-Puzzle problem using the specified
 * heuristic function and cost functions to calculate the cost of each node.
 * @param hChoice The choice of heuristic function to use.
 * @param totalStatesVisited A reference to an integer that will be updated with
 *                           the total number of states visited during the search.
 * @param maxStatesInMemory A reference to an integer that will be updated with
 *                          the maximum number of states held in memory at any
 *                          point during the search.
 * @param calculateG The cost function to use for calculating the cost of the
 *                   path from the start node to the current node.
 * @param calculateH The heuristic function to use for estimating the cost of
 *                   the path from the current node to the goal node.
 * @return A empty vector if no solution is found, otherwise a vector of nodes
 */
vector<Node *> A_star(int hChoice, int &totalStatesVisited, int &maxStatesInMemory, CostFunction calculateG, CostFunction calculateH) {
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

            int g = calculateG(*curr, tmp, hChoice);
            int h = calculateH(*curr, tmp, hChoice);
            int f = g + h;

            Node* newNode = new Node(tmp, moves[i], g, f); // New node with the new state and the corresponding f value
            newNode->parent = curr; // Set the parent pointer of the new node to the current node

            pq.push(newNode);
        }
    }
    
    return vector<Node*>(); // Return empty vector if no solution is found
}

/**
 * Performs an IDA* search algorithm (a variation of A* that uses iterative deepening).
 * @param curr The current node being explored.
 * @param g The cost of the path from the start node to the current node.
 * @param threshold The maximum cost allowed to explore a node.
 * @param hChoice The heuristic function to use.
 * @param totalStatesVisited The total number of states visited during the search.
 * @param maxStatesInMemory The maximum number of states held in memory during the search.
 * @param visitedStates A set of visited states to avoid exploring the same state twice.
 * @param path A vector of nodes representing the path from the start node to the current node.
 * @return The cost of the optimal solution or -1 if no solution is found.
 */
static int search(Node* curr, int g, int threshold, int hChoice, int &totalStatesVisited, int &maxStatesInMemory, unordered_set<vector<int>, VectorHash>& visitedStates, vector<Node*>& path) {
    
    if (visitedStates.find(curr->state) != visitedStates.end()) return numeric_limits<int>::max(); // Return a high value to prune this path since it has already been visited
    visitedStates.insert(curr->state); // Mark the current state as visited

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

        if (visitedStates.find(tmp) != visitedStates.end()) continue; // Check if the new state has already been visited to avoid revisiting previously explored states

        path.push_back(new Node(tmp, moves[i]));
        totalStatesVisited++;
        int currentStatesInMemory = path.size();
        if (currentStatesInMemory > maxStatesInMemory)
            maxStatesInMemory = currentStatesInMemory;

        int t = search(path.back(), g + 1, threshold, hChoice, totalStatesVisited, maxStatesInMemory, visitedStates, path);
        if (t == -1) // Solution found
            return -1;
        if (t < min)
            min = t;
        path.pop_back();
    }

    return min;
}

/**
 * Performs the IDA* search algorithm on the N-Puzzle problem.
 * @param hChoice An integer representing the heuristic function to use.
 * @param totalStatesVisited A reference to an integer that will be updated with the total number of states visited during the search.
 * @param maxStatesInMemory A reference to an integer that will be updated with the maximum number of states held in memory at any point during the search.
 * @return A vector of pointers to Node objects representing the solution path, or an empty vector if no solution was found.
 */
vector<Node *> IDA_star(int hChoice, int &totalStatesVisited, int &maxStatesInMemory) {
    vector<int> hello;
    for (auto row : puzzle->grid)
        for (auto e : row)
            hello.push_back(e);

    Node *root = new Node(hello, 'S');
    int threshold = hFunction(hChoice, root->state);

    while (true) {
        vector<Node *> path = { root }; // Store the path from the root node to the current node
        unordered_set<vector<int>, VectorHash> visitedStates; // Store the visited states to avoid revisiting previously explored states

        int t = search(root, 0, threshold, hChoice, totalStatesVisited, maxStatesInMemory, visitedStates, path);
        if (t == -1)
            return path;
        else if (t == numeric_limits<int>::max())
            return vector<Node *>();
        threshold = t;
    }
}
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <stack>
#include <unordered_set>
#include <chrono>
#include "npuzzle.hpp"
#include "HeuristicFunctions.hpp"

using namespace std;

int N = 3;
vector<vector<int>> goalState;

void printGrid(const std::vector<std::vector<int>>& grid) {
    for (const auto& row : grid) {
        for (int val : row) {
            std::cout << val << ' ';
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<int>> createSnail(int size) {
    auto res = std::vector<std::vector<int>>(size, std::vector<int>(size, 0));
    int val = 1; // The value to be inserted
    int rowStart = 0, rowEnd = size - 1;
    int colStart = 0, colEnd = size - 1;

    while (rowStart <= rowEnd && colStart <= colEnd) {
        for (int j = colStart; j <= colEnd; ++j) {
            res[rowStart][j] = val++;
        }
        ++rowStart;

        for (int i = rowStart; i <= rowEnd; ++i) {
            res[i][colEnd] = val++;
        }
        --colEnd;

        for (int j = colEnd; j >= colStart; --j) {
            res[rowEnd][j] = val++;
        }
        --rowEnd;

        for (int i = rowEnd; i >= rowStart; --i) {
            res[i][colStart] = val++;
        }
        ++colStart;
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (res[i][j] == size * size) {
                res[i][j] = 0;
                printGrid(res);
                return res;
            }
        }
    }
    return res;

}

struct pState {
    vector<vector<int>> board;
    int cost, hFunction;

    bool operator==(const pState &other) const {
        return board == other.board;
    }
};

bool isGoal(const pState &s) {
    return s.board == goalState;
}

int hFunction(const pState &s, int hChoice) {
    switch (hChoice) {
        case 1:
            return Manhattan(s.board, N);
        case 2:
            return Hamming(s.board, N);
        case 3:
            return Euclidean(s.board, N);
        default:
            return Manhattan(s.board, N);
    }
}

bool move(int dx, int dy, pState &s) {
    int x, y;
    for (x = 0; x < N; ++x) {
        for (y = 0; y < N; ++y)
            if (s.board[x][y] == 0)
                break ;
        if (y < N)
            break ;
    }

    int newX = x + dx;
    int newY = y + dy;

    if (newX >= 0 and newX < N and newY >= 0 and newY < N) {
        swap(s.board[x][y], s.board[newX][newY]);
        return true;
    }
    else
        return false;
}

// Threshold = bound
int search(pState &s, int g, int bound, int hChoice, vector<char> &solution) {
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
// launch 'make && <test ./n-puzzle'

int main(int ac, char *av[]) {
    // Smart Pointer
    std::unique_ptr<Puzzle> puzzle = parse(std::string(av[1]));

    // Check if it is well parsed
    std::cout << *puzzle << std::endl;

    pState initialState;
    initialState.board = puzzle->grid;

    N = puzzle->size;
    goalState = createSnail(puzzle->size);
    std::cout << "so " << getNumberAt(1, 1, puzzle->size) << endl;

    int hChoice;
    cout << "Choose a heuristic function (1->Manhattan, 2->Hamming, 3->Euclidean): " << endl;
    cin >> hChoice;
    cout << "Starting n-puzzle..." << endl;

    vector<char> solution;

    auto startTime = chrono::high_resolution_clock::now();

    int minMoves = IDAstar(initialState, hChoice, solution);

    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    if (minMoves == -1)
        cout << "No solution found" << endl;
    else {
        cout << "Number of moves required: " << minMoves << endl;
        cout << "Ordered sequence: ";
        for (char move : solution)
            cout << move << " ";
        cout << endl;
        cout << "Time taken: " << duration.count() << " milliseconds" << endl;
    }

    return 0;
}
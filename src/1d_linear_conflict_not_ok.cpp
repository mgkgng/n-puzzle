#include <iostream>
#include <vector>

using namespace std;

struct Puzzle {
    int size;
    vector<int> goalTest;
    vector<pair<int, int>> goalCoordinates;
};

int linearConflict(const vector<int>& state, const Puzzle& puzzle, vector<pair<int, int>>& conflictingTiles) {
    int h = 0;
    for (int i = 0; i < puzzle.size * puzzle.size; ++i) {
        int val = state[i];
        if (val != 0) {
            int goalRow = puzzle.goalCoordinates[val].first;
            int goalCol = puzzle.goalCoordinates[val].second;
            if (i / 3 != goalCol or i % 3 != goalRow) {
                for (int k = i + 1; k < puzzle.size * puzzle.size; ++k) {
                    int nextVal = state[k];
                    if (nextVal != 0 and (
                        puzzle.goalCoordinates[nextVal].first == goalRow && (i / 3 == puzzle.goalCoordinates[nextVal].first) or
                        puzzle.goalCoordinates[nextVal].second == goalCol && (i % 3 == puzzle.goalCoordinates[nextVal].second)
                        )) {
                        if (val > nextVal) {
                            h++;
                            conflictingTiles.push_back({val, nextVal});
                        }
                    }
                }
            }
        }
    }
    return h;
}

// Result should be 2 with conflicting tiles: (4, 1) (8, 7) -> NOT WORKING return (4, 1) (2, 1) (6, 3) (8, 7)
int main() {
    Puzzle puzzle;
    puzzle.size = 3;
    puzzle.goalTest = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    puzzle.goalCoordinates.resize(9);

    for (int i = 0; i < puzzle.size; ++i) {
        for (int j = 0; j < puzzle.size; ++j) {
            puzzle.goalCoordinates[puzzle.goalTest[i * puzzle.size + j]] = make_pair(i, j);
        }
    }

    vector<int> initialState = {4, 2, 5, 1, 0, 6, 3, 8, 7};

    vector<pair<int, int>> conflictingTiles;
    int linearConflictValue = linearConflict(initialState, puzzle, conflictingTiles);

    cout << "Linear Conflict Heuristic Value: " << linearConflictValue << endl;

    cout << "Conflicting tiles: ";
    for (const auto& tilePair : conflictingTiles) {
        cout << "(" << tilePair.first << ", " << tilePair.second << ") ";
    }
    cout << endl;

    return 0;
}

//// Result should be 3 with conflicting tiles: (2, 1) (5, 4) (5, 3) -> WORKING
//int main() {
//    Puzzle puzzle;
//    puzzle.size = 3;
//    puzzle.goalTest = {0, 1, 2, 3, 4, 5, 6, 7, 8};
//    puzzle.goalCoordinates.resize(9);

//    for (int i = 0; i < puzzle.size; ++i) {
//        for (int j = 0; j < puzzle.size; ++j) {
//            puzzle.goalCoordinates[puzzle.goalTest[i * puzzle.size + j]] = make_pair(i, j);
//        }
//    }

//    vector<int> initialState = {0, 2, 1, 5, 4, 3, 6, 7, 8};

//    vector<pair<int, int>> conflictingTiles;
//    int linearConflictValue = linearConflict(initialState, puzzle, conflictingTiles);

//    cout << "Linear Conflict Heuristic Value: " << linearConflictValue << endl;

//    cout << "Conflicting tiles: ";
//    for (const auto& tilePair : conflictingTiles) {
//        cout << "(" << tilePair.first << ", " << tilePair.second << ") ";
//    }
//    cout << endl;

//    return 0;
//}
#include <iostream>
#include <vector>

using namespace std;

int linearConflict(const vector<vector<int>>& state, const vector<pair<int, int>>& goalCoordinates, vector<pair<int, int>>& conflictingTiles) {
    int conflicts = 0;

    for (int i = 0; i < state.size(); ++i) {
        for (int j = 0; j < state[i].size(); ++j) {
            int val = state[i][j];
            if (val != 0) {
                int goalRow = goalCoordinates[val].first;
                int goalCol = goalCoordinates[val].second;
                if (i != goalCol or j != goalRow) {
                    for (int k = j + 1; k < state[i].size(); ++k) {
                        int nextVal = state[i][k];
                        if (nextVal != 0 && goalCoordinates[nextVal].first == goalRow && i == goalCoordinates[nextVal].first) {
                            if (val > nextVal) {
                                conflicts++;
                                conflictingTiles.push_back({val, nextVal});
                            }
                        }
                    }
                    for (int k = i + 1; k < state.size(); ++k) {
                        int nextVal = state[k][j];
                        if (nextVal != 0 && goalCoordinates[nextVal].second == goalCol && j == goalCoordinates[nextVal].second) {
                            if (val > nextVal) {
                                conflicts++;
                                conflictingTiles.push_back({val, nextVal});
                            }
                        }
                    }
                }
            }
        }
    }

    return conflicts;
}

// Result should be 2 with conflicting tiles: (4, 1) (8, 7) -> WORKING
int main() {
    vector<vector<int>> initialBoard = {{4, 2, 5}, {1, 0, 6}, {3, 8, 7}};
    vector<vector<int>> goalBoard = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};

    vector<pair<int, int>> goalCoordinates(9);

    for (int i = 0; i < goalBoard.size(); ++i) {
        for (int j = 0; j < goalBoard[i].size(); ++j) {
            goalCoordinates[goalBoard[i][j]] = make_pair(i, j);
        }
    }

    vector<pair<int, int>> conflictingTiles;

    int linearConflictValue = linearConflict(initialBoard, goalCoordinates, conflictingTiles);

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
//    vector<vector<int>> initialBoard = {{0, 2, 1}, {5, 4, 3}, {6, 7, 8}};
//    vector<vector<int>> goalBoard = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};

//    vector<pair<int, int>> goalCoordinates(9);

//    for (int i = 0; i < goalBoard.size(); ++i) {
//        for (int j = 0; j < goalBoard[i].size(); ++j) {
//            goalCoordinates[goalBoard[i][j]] = make_pair(i, j);
//        }
//    }

//    vector<pair<int, int>> conflictingTiles;

//    int linearConflictValue = linearConflict(initialBoard, goalCoordinates, conflictingTiles);

//    cout << "Linear Conflict Heuristic Value: " << linearConflictValue << endl;

//    cout << "Conflicting tiles: ";
//    for (const auto& tilePair : conflictingTiles) {
//        cout << "(" << tilePair.first << ", " << tilePair.second << ") ";
//    }
//    cout << endl;

//    return 0;
//}
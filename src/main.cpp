#include "puzzle.hpp"

unique_ptr<Puzzle> puzzle;

int main(int ac, char *av[]) {
    cout << endl;
    puzzle = parse(std::string(av[1]));

    cout << *puzzle << endl;

    if (not isSolvable(puzzle->initialGrid, puzzle->grid)) {
        cout << "The puzzle is not solvable!" << endl << endl;
        return 1;
    }

    int hChoice;
    cout << "Choose an heuristic function: [1]-Manhattan, [2]-Hamming, [3]-Euclidian, [4]-Manhattan + Linear Conflict" << endl;
    cin >> hChoice;
    cout << endl;

    int searchChoice;
    cout << "Choose a search algorithm: [1]-IDA*, [2]-A*:" << endl;
    cin >> searchChoice;
    cout << endl;

    cout << "Solving the puzzle..." << endl << endl;

    vector<char> solution;

    vector<Node*> path;

    auto startTime = chrono::high_resolution_clock::now();

    int totalStatesVisited = 0;
    int maxStatesInMemory = 0;

    if (searchChoice == 1)
        path = IDA_star(hChoice, totalStatesVisited, maxStatesInMemory);
    else if (searchChoice == 2)
        path = A_star(hChoice, totalStatesVisited, maxStatesInMemory);
    else {
        cout << "Invalid search algorithm choice!" << endl;
        return 1;
    }

    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    if (not path.size())
        cout << "No solution found!" << endl;
    else {
        cout << "Total states visited: " << totalStatesVisited << endl;
        cout << "Maximum number of states in memory: " << maxStatesInMemory << endl;
        cout << "Number of moves required: " << path.size() - 1<< endl;
        cout << "Ordered sequence: ";
        for (int i = 1; i < path.size(); i++)
            cout << path[i]->move << " ";
        cout << endl;
        cout << "Time taken: " << duration.count() << " milliseconds" << endl;
    }
    cout << endl;

    return 0;
}
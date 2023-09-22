#include "puzzle.hpp"

unique_ptr<Puzzle> puzzle;

int main(int ac, const char *av[]) {    
    puzzle = parse(ac, av);
    cout << *puzzle << endl;

    if (not isSolvable(puzzle->initialGrid, puzzle->grid)) {
        cout << "The puzzle is not solvable!" << endl << endl;
        return 1;
    }

    int searchChoice;
    while (true) {
        cout << "Search Algorithm:" << endl;
        cout << "1 -> A*" << endl;
        cout << "2 -> Iterative Deepening A* (IDA*)" << endl;
        cout << "Please type a number and press Enter: ";
        cin >> searchChoice;
        cout << endl;

        if (searchChoice == 1 || searchChoice == 2)
            break;
        else
            cout << "Invalid search algorithm choice! Please try again." << endl << endl;
    }

    int hChoice;
    while (true) {
        cout << "Heuristic Function:" << endl;
        cout << "1 -> Manhattan" << endl;
        cout << "2 -> Euclidean" << endl;
        cout << "3 -> Hamming" << endl;
        cout << "4 -> Linear Conflict" << endl;
        cout << "Please type a number and press Enter: ";
        cin >> hChoice;
        cout << endl;

        if (hChoice >= 1 && hChoice <= 4)
            break;
        else
            cout << "Invalid heuristic function choice! Please try again." << endl << endl;
    }

    int gChoice;
    if (searchChoice == 1) {
        while (true) {
            cout << "Search Type:" << endl;
            cout << "1 -> Default (Mandatory part)" << endl;
            cout << "2 -> Uniform-Cost (Bonus part)" << endl;
            cout << "3 -> Greedy Search (Bonus part)" << endl;
            cout << "Please type a number and press Enter: ";
            cin >> gChoice;
            cout << endl;

            if (gChoice >= 1 and gChoice <= 3)
                break;
            else
                cout << "Invalid search type choice! Please try again." << endl << endl;
        }
    }

    cout << "Solving the puzzle, please wait..." << endl << endl;

    vector<char> solution;

    vector<Node*> path;

    auto startTime = chrono::high_resolution_clock::now();

    int totalStatesVisited = 0;
    int maxStatesInMemory = 0;

    if (searchChoice == 1 and gChoice == 1)
        path = A_star(hChoice, totalStatesVisited, maxStatesInMemory, false, false);
    else if (searchChoice == 1 and gChoice == 2)
        path = A_star(hChoice, totalStatesVisited, maxStatesInMemory, true, false);
    else if (searchChoice == 1 and gChoice == 3)
        path = A_star(hChoice, totalStatesVisited, maxStatesInMemory, false, true);
    else if (searchChoice == 2)
        path = IDA_star(hChoice, totalStatesVisited, maxStatesInMemory);

    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    if (not path.size())
        cout << "No solution found!" << endl;
    else {
        cout << "Total states visited: " << totalStatesVisited << endl;
        cout << "Maximum number of states in memory: " << maxStatesInMemory << endl;
        cout << "Number of moves required: " << path.size() - 1<< endl;
        cout << "Ordered sequence: ";
        for (unsigned int i = 1; i < path.size(); i++)
            cout << path[i]->move << " ";
        //// Print the ordered sequence of moves and the corresponding grid
        //cout << "Ordered sequence: " << endl;
        //for (unsigned int i = 1; i < path.size(); i++) {
        //    cout << endl;
        //    cout << path[i]->move << ":";
        //    cout << endl;
        //    for (int j = 0; j < puzzle->size; j++) {
        //        for (int k = 0; k < puzzle->size; k++) {
        //            cout << setw(4) << path[i]->state[j * puzzle->size + k];
        //        }
        //        cout << endl;
        //    }
        //}
        cout << endl;
        cout << "Time taken: " << duration.count() << " milliseconds" << endl;
    }
    cout << endl;

    return 0;
}
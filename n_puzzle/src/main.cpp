#include "puzzle.hpp"

unique_ptr<Puzzle> puzzle;

int main(int ac, const char *av[]) {    
    int searchChoice = atoi(av[1]);
    int hChoice = atoi(av[2]);
    int gChoice = atoi(av[3]);

    puzzle = parse(ac, string(av[4]));

    if (not isSolvable(puzzle->initialGrid, puzzle->grid)) {
        cout << "ERR#" << "Puzzle not solvable." << endl << endl;
        return 1;
    }

    vector<char> solution;

    vector<Node*> path;

    auto startTime = chrono::high_resolution_clock::now();

    int totalStatesVisited = 0;
    int maxStatesInMemory = 0;

    std::cout << "search start" << std::endl;
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
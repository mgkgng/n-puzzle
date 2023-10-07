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
        cout << "ERR#" << "No solution found." << endl;
    else {
        cout << "TSV#" << totalStatesVisited << endl; // Total states visited
        cout << "MNS#" << maxStatesInMemory << endl; // Max number of states in memory
        const string solution = std::accumulate(path.begin() + 1, path.end(), std::string{},
            [](const std::string& acc, auto& p) { return acc + p->move; });
        cout << "SOL#" << string << endl; // Solution
        cout << "TME#" << duration.count() << endl; // Time elapsed
    }

    return 0;
}
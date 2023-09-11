#include "puzzle.hpp"

unique_ptr<Puzzle> puzzle;

int main(int ac, char *av[]) {
    puzzle = parse(std::string(av[1]));

    pState initialState;
    initialState.board = puzzle->grid;

    cout << *puzzle << endl;

    int hChoice;
    cout << "Choose a heuristic function (1->Manhattan, 2->Hamming, 3->Euclidean): " << endl;
    cin >> hChoice;
    cout << "Starting n-puzzle..." << endl;

    vector<char> solution;

    auto startTime = chrono::high_resolution_clock::now();

    // int minMoves = IDAstar(initialState, hChoice, solution);
    auto path = try_IDA(hChoice);

    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    if (!path.size()) {
        cout << "No solution found" << endl;
    } else {
        cout << "Number of moves required: " << path.size() - 1<< endl;
        cout << "Ordered sequence: ";
        for (auto p : path)
            cout << p->move << " ";
        cout << endl;
        cout << "Time taken: " << duration.count() << " milliseconds" << endl;
    }

    // if (minMoves == -1)
    //     cout << "No solution found" << endl;
    // else {
    //     cout << "Number of moves required: " << minMoves << endl;
    //     cout << "Ordered sequence: ";
    //     for (char move : solution)
    //         cout << move << " ";
    //     cout << endl;
    //     cout << "Time taken: " << duration.count() << " milliseconds" << endl;
    // }

    return 0;
}
#include "puzzle.hpp"

unique_ptr<Puzzle> puzzle;

int main(int ac, char *av[]) {
    cout << endl;
    puzzle = parse(std::string(av[1]));

    // Print inversion count in the initial grid
    //cout << "Inversion count: " << invCount(puzzle->initialGrid) << endl;
    //// Print X position in the initial grid
    //cout << "X pos: " << findXPos(puzzle->grid) << endl;

    //cout << "Initial grid:" << endl;
    //for (int val : puzzle->initialGrid) {
    //    cout << setw(4) << val;
    //}
    //cout << endl;

    //cout << "Goal grid:" << endl;
    //for (int val : puzzle->goalTest) {
    //    cout << setw(4) << val;
    //}
    //cout << endl;

    cout << *puzzle << endl;

    if (!isSolvable(puzzle->initialGrid, puzzle->grid)) {
        cout << "Puzzle is not solvable!" << endl << endl;
        return 0;
    }

    int hChoice;
    cout << "Choose an heuristic function: 1->Manhattan, 2->Hamming, 3->Linear Conflict" << endl;
    cin >> hChoice;
    cout << endl;

    int searchChoice;
    cout << "Choose a search algorithm: 1->A*, 2->IDA*:" << endl;
    cin >> searchChoice;
    cout << endl;

    cout << "Solving the N-Puzzle..." << endl << endl;

    vector<char> solution;

    vector<Node*> path;

    auto startTime = chrono::high_resolution_clock::now();

    if (searchChoice == 1)
        path = A_star(hChoice);
    else if (searchChoice == 2)
        path = IDA_star(hChoice);
    else {
        cout << "Invalid search algorithm choice." << endl;
        return 1;
    }

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
    cout << endl;

    return 0;
}
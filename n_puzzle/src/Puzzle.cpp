#include "Puzzle.hpp"

Puzzle::Puzzle() {}

Puzzle::Puzzle(int _size) {
	size = _size;
	grid = vector<vector<int>>(_size, vector<int>(_size, 0));
	goalGrid = createSnail(size);

	for (auto row : goalGrid) {
		for (auto e : row)
			goalTest.push_back(e);
	}

	goalCoordinates = vector<pair<int, int>>(size * size);
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			int val = goalGrid[i][j];
			goalCoordinates[val] = {i, j};
		}
	}
}

Puzzle::~Puzzle() {}

ostream& operator<<(ostream& os, const Puzzle& p) {
    os << "Initial Grid (" << p.size << "x" << p.size << "):"<< endl;
    for (const auto& row : p.grid) {
        for (const auto& cell : row)
            os << setw(4) << cell;
        os << endl;
    }
    return os;
}
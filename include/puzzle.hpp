#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

class Puzzle {
    public:
        int size;
        std::vector<std::vector<int>> grid;

    Puzzle() {}
    Puzzle(int _size) {
        size = _size;
        grid = std::vector<std::vector<int>>(_size, std::vector<int>(_size, 0));
    }
    ~Puzzle() {}

    friend std::ostream& operator<<(std::ostream& os, const Puzzle& p);
};

inline std::ostream& operator<<(std::ostream& os, const Puzzle& p) {
    os << "Puzzle size: " << p.size << std::endl;
    for (const auto& row : p.grid) {
        for (const auto& cell : row) {
            os << std::setw(4) << cell;
        }
        os << std::endl;
    }
    return os;
}
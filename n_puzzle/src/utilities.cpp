#include "utilities.hpp"

size_t VectorHash::operator()(const vector<int>& vec) const {
    hash<int> hasher;
    size_t seed = 0;
    for (int i : vec)
        seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
}

vector<string> split(const string &str) {
    istringstream iss(str);
    vector<string> res;

    string token;
    while (iss >> token)
        res.push_back(token);
    return res;
}

int myAbs(int x) {
    return (x < 0) ? -x : x;
}

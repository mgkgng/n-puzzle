#pragma once

#include <vector>

using namespace std;

struct Node {
    vector<int> state;
    char move;
    int g;
    int f;
    Node* parent;

    Node(vector<int> _state, char _move);
    Node(vector<int> _state, char _move, int _g, int _f);
};

struct CompareNodes {
    bool operator()(const Node* left, const Node* right) const {
        return left->f > right->f;
    }
};
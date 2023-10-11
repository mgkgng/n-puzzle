#include "Node.hpp"

Node::Node(vector<int> _state, char _move) {
    state = _state;
    move = _move;
}

Node::Node(vector<int> _state, char _move, int _g, int _f) {
    state = _state;
    move = _move;
    g = _g;
    f = _f;
    parent = nullptr;
}
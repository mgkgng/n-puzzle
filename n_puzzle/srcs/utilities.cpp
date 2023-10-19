#include "headers.hpp"

int myAbs(int x) {
    return (x < 0) ? -x : x;
}

// https://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html
size_t VectorHash::operator()(const vector<int>& vec) const { // Overload of the () operator (a function operator) for the class VectorHash
    hash<int> hasher; // Initialize a standard hash function for integers
    size_t seed = 0; // Initialize the hash seed value to zero

    for (int i : vec) // Loop through each element in the input vector
        // Combine the current seed with the hashed value of the vector element
        // Use bitwise operations and addition to mix the bits and create the final hash
        seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2); // Update the seed

    return seed; // Return the final hash value
}
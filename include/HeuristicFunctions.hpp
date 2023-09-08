#pragma once

#include <vector>

int Manhattan(const std::vector<std::vector<int>>& state, int N, const std::vector<std::vector<int>> &goalGrid);
int Hamming(const std::vector<std::vector<int>>& state, int N, const std::vector<std::vector<int>> &goalGrid);
int Euclidean(const std::vector<std::vector<int>>& state, int N, const std::vector<std::vector<int>> &goalGrid);
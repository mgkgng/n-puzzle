#pragma once

#include <sstream>
#include <vector>
#include <string>

std::vector<std::string> split(const std::string &str) {
    std::istringstream iss(str);
    std::vector<std::string> res;

    std::string token;
    while (iss >> token)
        res.push_back(token);
    return res;
}

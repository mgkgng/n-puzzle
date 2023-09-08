#pragma once

#include <fstream>
#include <cstdlib>
#include "puzzle.hpp"

std::unique_ptr<Puzzle> parse(const std::string &filename);

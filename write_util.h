#pragma once

#include <vector>
#include <string_view>
#include "json.hpp"
#include "sudoku.h"

using json = nlohmann::json;

void writeToFile(std::unordered_map<int, StrBoard>* bs, std::string& fileName);


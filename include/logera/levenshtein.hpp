#pragma once

#include <string_view>
#include <vector>

class levenshtein
{
    std::vector<int> m_matrix;

public:
    auto distance(std::string_view const lhs, std::string_view const rhs) -> int;
};

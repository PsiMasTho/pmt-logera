//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string_view>
#include <vector>

class levenshtein
{
    std::vector<int> m_matrix;

public:
    auto distance(std::string_view const lhs, std::string_view const rhs) -> int;
};

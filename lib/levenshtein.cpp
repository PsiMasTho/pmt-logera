//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "logera/levenshtein.hpp"

#include <algorithm>
#include <numeric>

using namespace std;

auto levenshtein::distance(string_view const lhs, string_view const rhs) -> size_t
{
    if (lhs.size() > rhs.size())
        return distance(rhs, lhs);

    size_t const min_size = lhs.size(), max_size = rhs.size();
    m_matrix.resize(min_size + 1);

    iota(m_matrix.begin(), m_matrix.end(), 0);

    for (size_t j = 1; j <= max_size; ++j)
    {
        size_t previous_diagonal = m_matrix[0];
        ++m_matrix[0];

        for (size_t i = 1; i <= min_size; ++i)
        {
            size_t const previous_diagonal_save = m_matrix[i];
            if (lhs[i - 1] == rhs[j - 1])
                m_matrix[i] = previous_diagonal;
            else
                m_matrix[i] = std::min(std::min(m_matrix[i - 1], m_matrix[i]), previous_diagonal) + 1;

            previous_diagonal = previous_diagonal_save;
        }
    }

    return m_matrix[min_size];
}

#include "levenshtein.hpp"

#include <algorithm>

using namespace std;

auto levenshtein::distance(string_view const lhs, string_view const rhs) -> int
{
    int const sizes[2] = { static_cast<int>(lhs.size()), static_cast<int>(rhs.size()) };

    if (!sizes[0] || !sizes[1])
        return max(sizes[0], sizes[1]);

    m_matrix.resize((sizes[0] + 1) * (sizes[1] + 1));

    auto const mat2d = [x_max = sizes[0], this](int x, int y) -> int& { return m_matrix[x * x_max + y]; };

    for (int x = 0; x <= sizes[0]; ++x)
        mat2d(x, 0) = x;
    for (int y = 0; y <= sizes[1]; ++y)
        mat2d(0, y) = y;

    for (int x = 1; x <= sizes[0]; ++x)
    {
        for (int y = 1; y <= sizes[1]; ++y)
        {
            int const cost = (lhs[x - 1] != rhs[y - 1]);
            mat2d(x, y)    = min(min(mat2d(x - 1, y) + 1, mat2d(x, y - 1) + 1), mat2d(x - 1, y - 1) + cost);
        }
    }

    return mat2d(sizes[0], sizes[1]);
}

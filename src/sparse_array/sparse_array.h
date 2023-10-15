//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_SPARSE_ARRAY_H
#define INCLUDED_SPARSE_ARRAY_H

#include <cstddef>
#include <vector>

template <typename T>
class SparseArray
{
    std::vector<T> m_values;
    std::vector<bool> m_specified;

public:
    explicit SparseArray(size_t max);
    void set(size_t idx, T const& value);
    void set(size_t idx, T&& value);
    bool exists(size_t idx) const;
    T const& get(size_t idx) const;

    size_t capacity() const;

private:
    template <typename U>
    void _set(size_t idx, U&& value);
};

#include "sparse_array-inl.h"

#endif
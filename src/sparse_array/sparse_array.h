#ifndef INCLUDED_SPARSE_ARRAY_H
#define INCLUDED_SPARSE_ARRAY_H

#include <vector>
#include <cstddef>

template <typename T>
class SparseArray
{
    std::vector<T> d_values;
    std::vector<bool> d_specified;

public:
    explicit SparseArray(size_t max);
    void     set(size_t idx, T value);
    bool     exists(size_t idx) const;
    T const& get(size_t idx) const;

    size_t capacity() const;
};

#include "sparse_array.hi"

#endif
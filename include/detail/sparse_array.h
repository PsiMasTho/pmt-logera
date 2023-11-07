//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstddef>
#include <vector>
#include <algorithm>

template <typename T>
class sparse_array
{
    std::vector<T> m_values;
    std::vector<bool> m_specified;

public:
    explicit sparse_array(size_t max);
    void set(size_t idx, T const& value);
    void set(size_t idx, T&& value);
    bool exists(size_t idx) const;
    T const& get(size_t idx) const;

    size_t capacity() const;

private:
    template <typename U>
    void _set(size_t idx, U&& value);
};

template <typename T>
sparse_array<T>::sparse_array(size_t max)
    : m_values{}
    , m_specified(max, false)
{ }

template <typename T>
void sparse_array<T>::set(size_t idx, T const& value)
{
    _set(idx, value);
}

template <typename T>
void sparse_array<T>::set(size_t idx, T&& value)
{
    _set(idx, std::move(value));
}

template <typename T>
bool sparse_array<T>::exists(size_t idx) const
{
    return m_specified[idx];
}

template <typename T>
T const& sparse_array<T>::get(size_t idx) const
{
    auto const cItr =
        cbegin(m_values) + std::count(cbegin(m_specified),
                                      cbegin(m_specified) + static_cast<std::ptrdiff_t>(idx),
                                      true);
    return *cItr;
}

template <typename T>
size_t sparse_array<T>::capacity() const
{
    return m_specified.size();
}

template <typename T>
template <typename U>
void sparse_array<T>::_set(size_t idx, U&& value)
{
    auto const countRhs =
        std::count(cbegin(m_specified) + static_cast<std::ptrdiff_t>(idx), cend(m_specified), true);
    m_values.insert(cend(m_values) - countRhs, std::forward<U>(value));
    m_specified[idx] = true;
}
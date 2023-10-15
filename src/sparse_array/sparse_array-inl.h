//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>

template <typename T>
SparseArray<T>::SparseArray(size_t max)
    : m_values{}
    , m_specified(max, false)
{ }

template <typename T>
void SparseArray<T>::set(size_t idx, T const& value)
{
    _set(idx, value);
}

template <typename T>
void SparseArray<T>::set(size_t idx, T&& value)
{
    _set(idx, std::move(value));
}

template <typename T>
bool SparseArray<T>::exists(size_t idx) const
{
    return m_specified[idx];
}

template <typename T>
T const& SparseArray<T>::get(size_t idx) const
{
    auto const cItr =
        cbegin(m_values) + std::count(cbegin(m_specified),
                                      cbegin(m_specified) + static_cast<std::ptrdiff_t>(idx),
                                      true);
    return *cItr;
}

template <typename T>
size_t SparseArray<T>::capacity() const
{
    return m_specified.size();
}

template <typename T>
template <typename U>
void SparseArray<T>::_set(size_t idx, U&& value)
{
    auto const countRhs =
        std::count(cbegin(m_specified) + static_cast<std::ptrdiff_t>(idx), cend(m_specified), true);
    m_values.insert(cend(m_values) - countRhs, std::forward<U>(value));
    m_specified[idx] = true;
}
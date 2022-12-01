#include <algorithm>

template <typename T>
SparseArray<T>::SparseArray(size_t max)
    : d_values{}
    , d_specified(max, false)
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
    return d_specified[idx];
}

template <typename T>
T const& SparseArray<T>::get(size_t idx) const
{
    auto const cItr =
        cbegin(d_values) + std::count(cbegin(d_specified),
                                      cbegin(d_specified) + static_cast<std::ptrdiff_t>(idx),
                                      true);
    return *cItr;
}

template <typename T>
size_t SparseArray<T>::capacity() const
{
    return d_specified.size();
}

template <typename T>
template <typename U>
void SparseArray<T>::_set(size_t idx, U&& value)
{
    auto const countRhs =
        std::count(cbegin(d_specified) + static_cast<std::ptrdiff_t>(idx), cend(d_specified), true);
    d_values.insert(cend(d_values) - countRhs, std::forward<U>(value));
    d_specified[idx] = true;
}
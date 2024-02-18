// clang-format off
#ifdef __INTELLISENSE__
    #include "algo.hpp"
#endif
// clang-format on

#include <algorithm>
#include <iterator>

namespace algo
{

template <typename T, typename U>
    requires(std::is_integral_v<typename U::value_type>)
auto indirect_rearrange(T data_begin, T data_end, U indices_begin, std::unique_ptr<typename U::value_type[]> buf)
{
    using index_type      = typename U::value_type;
    index_type const size = distance(data_begin, data_end);

    buf = buf ? std::move(buf) : std::make_unique_for_overwrite<typename U::value_type[]>(size);

    std::copy(indices_begin, std::next(indices_begin, size), buf.get());

    for (index_type idx1 = 0; idx1 < size - 1; ++idx1)
    {
        if (buf[idx1] == idx1)
            continue;
        index_type idx2;
        for (idx2 = idx1 + 1; idx2 < size; ++idx2)
            if (buf[idx2] == idx1)
                break;

        // make a special case when the data is in vector<bool>
        // because std::swap is not specialized for vector<bool> proxies
        if constexpr (std::is_same_v<typename std::iterator_traits<T>::value_type, bool>)
            std::vector<bool>::swap(data_begin[idx1], data_begin[buf[idx1]]);
        else
            std::swap(data_begin[idx1], data_begin[buf[idx1]]);

        std::swap(buf[idx1], buf[idx2]);
    }

    return buf;
}

template <typename T, typename Pred> auto duplicates(T first, T last, Pred&& pred) -> std::vector<T>
{
    std::vector<T> ret;

    while (first != last)
    {
        T const next
            = std::find_if(first, last, [first, &pred](auto const& x) { return !std::forward<Pred>(pred)(*first, x); });

        if (std::distance(first, next) > 1)
            for (auto i = first; i != next; std::advance(i, 1))
                ret.push_back(i);

        first = next;
    }

    return ret;
}

template <typename T, typename Pred>
auto duplicates_v(T first, T last, Pred&& pred) -> std::vector<typename T::value_type>
{
    auto const                          dup = duplicates(first, last, std::forward<Pred>(pred));
    std::vector<typename T::value_type> ret;
    ret.reserve(dup.size());
    for (auto const& ptr : dup)
        ret.push_back(*ptr);
    return ret;
}

template <typename T, typename Pred> auto excess_duplicates(T first, T last, Pred&& pred) -> std::vector<T>
{
    std::vector<T> ret;

    while (first != last)
    {
        T lookahead = first + 1;
        for (; lookahead != last; std::advance(lookahead, 1))
        {
            if (!std::forward<Pred>(pred)(*first, *lookahead))
                break;
            ret.push_back(lookahead);
        }
        first = lookahead;
    }

    return ret;
}

template <typename T, typename Pred>
auto excess_duplicates_v(T first, T last, Pred&& pred) -> std::vector<typename T::value_type>
{
    auto const                          excess_dup = excess_duplicates(first, last, std::forward<Pred>(pred));
    std::vector<typename T::value_type> ret;
    ret.reserve(excess_dup.size());
    for (auto const& ptr : excess_dup)
        ret.push_back(*ptr);
    return ret;
}

} // namespace algo

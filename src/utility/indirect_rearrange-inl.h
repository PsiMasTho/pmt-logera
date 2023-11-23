// clang-format off
#ifdef __INTELLISENSE__
    #include "utility.h"
#endif
// clang-format on

#include <vector>
#include <algorithm>
#include <iterator>

template <typename T, typename U>
    requires(std::is_integral_v<typename U::value_type>)
auto indirect_rearrange(T data_begin, T data_end, U indices_begin, std::unique_ptr<typename U::value_type[]> buf)
{
    using index_type = typename U::value_type;
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
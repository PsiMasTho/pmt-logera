//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <algorithm>
#include <iterator>
#include <memory>
#include <type_traits>
#include <vector>

/*
    This function shuffles the elements of the range [data_begin, data_end)
    according to the indices in the range [indices_begin, indices_end).

    Because it is intended to be used in indirect sorting, where the indices
    are often calculated once and then reused to sort many ranges, the indices
    are stored in a buffer and reused if provided.
*/

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

#include <string>

/*
    Replaces all occurance of to_erase in target with to_replace iteratively.
*/
void erase_and_replace(std::string* target, std::string const& to_erase, std::string const& to_replace);
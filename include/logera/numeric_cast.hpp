//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cassert>
#include <concepts>

/// @brief Casts a value from one integral type to another, ensuring that the
/// value is preserved. Check is only performed in debug mode. Similar to
/// boost::numeric_cast.
/// @tparam T The target integral type.
/// @tparam U The source integral type.
template <std::integral T, std::integral U>
auto numeric_cast(U from) -> T
{
    T result = static_cast<T>(from);
    assert(static_cast<U>(result) == from && (result >= T()) == (from >= U()));
    return result;
}
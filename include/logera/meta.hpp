//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <tuple>
#include <utility>
#include <variant>

namespace meta
{

template <typename T, typename Seq>
struct expander;

template <typename T, std::size_t... Is>
struct expander<T, std::index_sequence<Is...>>
{
    template <typename E, std::size_t>
    using elem = E;

    using type = std::tuple<elem<T, Is>...>;
};

template <std::size_t N, class T>
struct repeat_tuple
{
    using type = typename expander<T, std::make_index_sequence<N>>::type;
};

template <typename>
struct tag
{
};

template <typename T, typename V>
struct get_index;

template <typename T, typename... Ts>
struct get_index<T, std::variant<Ts...>>
    : std::integral_constant<std::size_t, std::variant<tag<Ts>...>(tag<T>()).index()>
{
};

} // namespace meta

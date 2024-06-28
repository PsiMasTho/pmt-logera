#pragma once

#include <chrono>
#include <optional>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

namespace pmt::traits
{
template <typename... T>
struct is_vector : std::false_type
{
};

template <typename... T>
struct is_vector<std::vector<T...>> : std::true_type
{
};

template <typename... T>
struct is_tuple : std::false_type
{
};

template <typename... T>
struct is_tuple<std::tuple<T...>> : std::true_type
{
};

template <typename T>
struct is_optional : std::false_type
{
};

template <typename T>
struct is_optional<std::optional<T>> : std::true_type
{
};

template <typename T>
struct is_variant : std::false_type
{
};

template <typename... TS>
struct is_variant<std::variant<TS...>> : std::true_type
{
};

template <typename T>
struct is_duration : std::false_type
{
};

template <typename REP, typename PERIOD>
struct is_duration<std::chrono::duration<REP, PERIOD>> : std::true_type
{
};

} // namespace pmt::traits
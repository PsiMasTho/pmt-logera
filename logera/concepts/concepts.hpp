#pragma once

#include "logera/type_traits/type_traits.hpp"

namespace pmt::ccpt
{

template <typename T>
concept is_vector = traits::is_vector<T>::value;

template <typename T>
concept is_tuple = traits::is_tuple<T>::value;

template <typename T>
concept is_optional = traits::is_optional<T>::value;

template <typename T>
concept is_variant = traits::is_variant<T>::value;

template <typename T>
concept is_duration = traits::is_duration<T>::value;

} // namespace pmt::ccpt
#pragma once

#include "logera/string_literal/string_literal.hpp"

#include <type_traits>

namespace pmt
{

class named_class_tag
{
};

template <string_literal name_>
struct named_class : private named_class_tag
{
  static constexpr string_literal _name = name_;
};

template <typename T>
concept is_named_class = std::is_base_of_v<named_class_tag, std::remove_cvref_t<T>>;

} // namespace pmt
#pragma once

#include <cstdint>

namespace pmt
{

template <std::size_t n_>
struct string_literal
{
  constexpr string_literal(char const (&str_)[n_]);
  static constexpr std::size_t _size = n_ - 1;
  char                         _value[n_];
};

} // namespace pmt

#include "string_literal-inl.hpp"
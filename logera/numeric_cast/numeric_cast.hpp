#pragma once

#include <cassert>
#include <concepts>

namespace pmt
{

/// \brief Casts a value from one integral type to another, ensuring that the
/// value is preserved. Check is only performed in debug mode.
template <std::integral To, std::integral From>
auto numeric_cast(From from_) -> To
{
  To result = static_cast<To>(from_);
  assert(static_cast<From>(result) == from_ && (result >= To()) == (from_ >= From()));
  return result;
}

} // namespace pmt
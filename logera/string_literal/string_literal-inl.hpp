// clang-format off
#ifdef __INTELLISENSE__
    #include "string_literal.hpp"
#endif
// clang-format on

#include <algorithm>

namespace pmt
{

template <std::size_t n_>
constexpr string_literal<n_>::string_literal(char const (&str_)[n_])
{
  std::copy_n(str_, n_, _value);
}

} // namespace pmt

// clang-format off
#ifdef __INTELLISENSE__
    #include "str_cat.hpp"
#endif
// clang-format on

namespace pmt
{

template <string_castable... TS>
auto str_cat(TS&&... first_) -> std::string
{
  return (string_cast(std::forward<TS>(first_)) + ...);
}

} // namespace pmt
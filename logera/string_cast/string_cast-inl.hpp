// clang-format off
#include <chrono>
#ifdef __INTELLISENSE__
    #include "string_cast.hpp"
#endif
// clang-format on

#include <sstream>
#include <utility>

namespace pmt
{

PMT_STRING_CAST_DECL_TEMPLATE(typename)
{
  if constexpr (requires { std::string{ x_ }; })
    return std::string{ x_ };

  return std::to_string(x_);
}

PMT_STRING_CAST_DECL_TEMPLATE(ccpt::is_vector)
{
  std::string str = "[";
  std::string delim;
  for (auto& elem : x_)
    str += std::exchange(delim, ", ") + string_cast(std::move(elem));
  str += "]";

  return str;
}

PMT_STRING_CAST_DECL_TEMPLATE(ccpt::is_duration)
{
  std::stringstream ss;
  ss << x_;
  return ss.str();
}

PMT_STRING_CAST_DECL(std::filesystem::path const&)
{
  return x_.string();
}

} // namespace pmt
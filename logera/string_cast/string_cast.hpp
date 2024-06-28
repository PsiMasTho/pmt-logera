#pragma once

#include "logera/concepts/concepts.hpp"

#include <filesystem>
#include <string>
#include <string_view>

#define PMT_STRING_CAST_DECL_TEMPLATE(TYPENAME) \
  template <TYPENAME T>                         \
  auto string_cast(T x_)->std::string

#define PMT_STRING_CAST_DECL(T) \
  template <>                   \
  auto string_cast<T>(T x_)->std::string

namespace pmt
{

template <typename T>
concept string_castable = requires(T x_) {
  {
    string_cast(x_)
  } -> std::same_as<std::string>;
};

PMT_STRING_CAST_DECL_TEMPLATE(typename);
PMT_STRING_CAST_DECL_TEMPLATE(ccpt::is_vector);
PMT_STRING_CAST_DECL_TEMPLATE(ccpt::is_duration);
PMT_STRING_CAST_DECL(std::filesystem::path const&);

} // namespace pmt

#include "string_cast-inl.hpp"
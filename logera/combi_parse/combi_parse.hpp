#pragma once

#include <optional>
#include <string_view>
#include <tuple>
#include <variant>
#include <vector>

namespace pmt
{

template <typename CONTEXT>
struct combi_parse
{
  template <typename... T>
  struct sequence
  {
    using return_type = std::tuple<typename T::return_type...>;
    auto operator()(CONTEXT&) -> std::optional<return_type>;
  };

  template <typename T>
  struct one_or_more
  {
    using return_type = std::vector<typename T::return_type>;
    auto operator()(CONTEXT&) -> std::optional<return_type>;
  };

  template <typename T>
  struct zero_or_more
  {
    using return_type = std::vector<typename T::return_type>;
    auto operator()(CONTEXT&) -> std::optional<return_type>;
  };

  template <typename... T>
  struct one_of
  {
    using return_type = std::variant<typename T::return_type...>;
    auto operator()(CONTEXT&) -> std::optional<return_type>;
  };

  template <typename T>
  struct maybe
  {
    using return_type = std::optional<typename T::return_type>;
    auto operator()(CONTEXT&) -> std::optional<return_type>;
  };

  template <typename T>
  struct action
  {
    struct return_type
    {
    };
    auto operator()(CONTEXT&) -> std::optional<return_type>;
  };

  template <typename DERIVED, typename OUT, typename T>
  struct converter
  {
    using input_type  = typename T::return_type;
    using return_type = OUT;
    auto operator()(CONTEXT&) -> std::optional<return_type>;
  };
};

} // namespace pmt

#include "combi_parse-inl.hpp"
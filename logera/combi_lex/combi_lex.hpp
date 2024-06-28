#pragma once

#include "logera/string_literal/string_literal.hpp"

#include <string_view>

namespace pmt
{

struct combi_lex
{
  class context
  {
  public:
    explicit context(std::string_view str_);

    void set_cursor(char const* cursor_);

    void move_cursor(int offset_);

    auto get_match() const -> std::string_view;
    auto get_remaining() const -> std::string_view;
    auto get_cursor() const -> char const*;
    auto get_end() const -> char const*;

  private:
    std::string_view _str;
    char const*      _cursor;
  };

  template <typename... T>
  struct sequence
  {
    auto operator()(context&) const -> bool;
  };

  template <typename T>
  struct zero_or_more
  {
    auto operator()(context&) const -> bool;
  };

  template <typename T, std::size_t n_>
  struct exactly_n
  {
    auto operator()(context&) const -> bool;
  };

  template <typename T, std::size_t n_>
  struct n_or_more
  {
    auto operator()(context&) const -> bool;
  };

  template <typename T>
  struct one_or_more
  {
    auto operator()(context&) const -> bool;
  };

  template <typename... T>
  struct one_of
  {
    auto operator()(context&) const -> bool;
  };

  template <char c_>
  struct match_char
  {
    auto operator()(context&) const -> bool;
  };

  template <char min_, char max_>
  struct match_in_range
  {
    static_assert(min_ < max_, "Invalid range");
    auto operator()(context&) const -> bool;
  };

  template <string_literal literal_>
  struct match_in_group
  {
    auto operator()(context&) const -> bool;
  };

  template <string_literal literal_>
  struct match_not_in_group
  {
    auto operator()(context&) const -> bool;
  };

  template <string_literal literal_>
  struct match_literal
  {
    auto operator()(context&) const -> bool;
  };

  struct match_any
  {
    auto operator()(context&) const -> bool;
  };
};

} // namespace pmt

#include "combi_lex-inl.hpp"
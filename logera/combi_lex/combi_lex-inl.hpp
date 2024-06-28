// clang-format off
#ifdef __INTELLISENSE__
    #include "combi_lex.hpp"
#endif
// clang-format on

#include "logera/meta/meta.hpp"

#include <algorithm>

namespace pmt
{

combi_lex::context::context(std::string_view str_)
  : _str{ str_ }
  , _cursor{ str_.data() }
{
}

void combi_lex::context::set_cursor(char const* cursor_)
{
  _cursor = cursor_;
}

void combi_lex::context::move_cursor(int offset_)
{
  _cursor += offset_;
}

auto combi_lex::context::get_match() const -> std::string_view
{
  return { _str.data(), static_cast<size_t>(_cursor - _str.data()) };
}

auto combi_lex::context::get_remaining() const -> std::string_view
{
  return std::string_view(_cursor, _str.end());
}

auto combi_lex::context::get_cursor() const -> char const*
{
  return _cursor;
}

auto combi_lex::context::get_end() const -> char const*
{
  return _str.end();
}

template <typename... T>
auto combi_lex::sequence<T...>::operator()(context& ctx_) const -> bool
{
  auto const backtrack = ctx_.get_cursor();

  if ((... && T{}(ctx_)))
    return true;
  else
  {
    ctx_.set_cursor(backtrack);
    return false;
  }
}

template <typename T>
auto combi_lex::zero_or_more<T>::operator()(context& ctx_) const -> bool
{
  auto backtrack = ctx_.get_cursor();

  auto iterate = [&]() -> bool
  {
    if (T{}(ctx_))
    {
      backtrack = ctx_.get_cursor();
      return true;
    }
    else
      return false;
  };

  while (iterate())
    continue;

  ctx_.set_cursor(backtrack);

  return true;
}

template <typename T, std::size_t n_>
auto combi_lex::exactly_n<T, n_>::operator()(context& ctx_) const -> bool
{
  return expand_to_template<sequence, T, n_>()(ctx_);
}

template <typename T, std::size_t n_>
auto combi_lex::n_or_more<T, n_>::operator()(context& ctx_) const -> bool
{
  return sequence<exactly_n<T, n_>, zero_or_more<T>>{}(ctx_);
}

template <typename T>
auto combi_lex::one_or_more<T>::operator()(context& ctx_) const -> bool
{
  return n_or_more<T, 1>{}(ctx_);
}

template <typename... T>
auto combi_lex::one_of<T...>::operator()(context& ctx_) const -> bool
{
  auto const backtrack = ctx_.get_cursor();

  if ((... || T{}(ctx_)))
    return true;

  ctx_.set_cursor(backtrack);
  return false;
}

template <char c_>
auto combi_lex::match_char<c_>::operator()(context& ctx_) const -> bool
{
  if (ctx_.get_cursor() == ctx_.get_end())
    return false;

  if (*ctx_.get_cursor() != c_)
    return false;

  ctx_.move_cursor(1);
  return true;
}

template <char min_, char max_>
auto combi_lex::match_in_range<min_, max_>::operator()(context& ctx_) const -> bool
{
  if (ctx_.get_cursor() == ctx_.get_end())
    return false;

  if (*ctx_.get_cursor() < min_ || *ctx_.get_cursor() > max_)
    return false;

  ctx_.move_cursor(1);
  return true;
}

template <string_literal literal_>
auto combi_lex::match_in_group<literal_>::operator()(context& ctx_) const -> bool
{
  if (ctx_.get_cursor() == ctx_.get_end())
    return false;

  for (size_t i = 0; i < literal_._size; ++i)
  {
    if (*ctx_.get_cursor() == literal_._value[i])
    {
      ctx_.move_cursor(1);
      return true;
    }
  }

  return false;
}

template <string_literal literal_>
auto combi_lex::match_not_in_group<literal_>::operator()(context& ctx_) const -> bool
{
  if (ctx_.get_cursor() == ctx_.get_end())
    return false;

  for (size_t i = 0; i < literal_._size; ++i)
    if (*ctx_.get_cursor() == literal_._value[i])
      return false;

  ctx_.move_cursor(1);
  return true;
}

template <string_literal literal_>
auto combi_lex::match_literal<literal_>::operator()(context& ctx_) const -> bool
{
  std::string_view const remaining = ctx_.get_remaining();

  if (remaining.substr(0, literal_._size) != literal_._value)
    return false;

  ctx_.move_cursor(literal_._size);
  return true;
}

auto combi_lex::match_any::operator()(context& ctx_) const -> bool
{
  if (ctx_.get_cursor() == ctx_.get_end())
    return false;

  ctx_.move_cursor(1);
  return true;
}

} // namespace pmt

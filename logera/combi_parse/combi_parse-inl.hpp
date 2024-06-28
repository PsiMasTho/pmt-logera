// clang-format off
#ifdef __INTELLISENSE__
    #include "combi_parse.hpp"
#endif
// clang-format on

namespace pmt
{

template <typename CONTEXT>
template <typename... T>
auto combi_parse<CONTEXT>::sequence<T...>::operator()(CONTEXT& ctx_) -> std::optional<return_type>
{
  return_type result;
  auto        backtrack = ctx_.capture_state();
  std::size_t count     = 0;

  auto fold_impl = [&]<std::size_t index_>(auto& parser_) -> bool
  {
    if (auto res = parser_(ctx_); res.has_value())
    {
      std::get<index_>(result) = std::move(*res);
      ++count;
      return true;
    }
    else
      return false;
  };

  std::tuple<T...> combinators;
  auto             index_over = [&]<std::size_t... i_>(std::index_sequence<i_...>)
  { (... && fold_impl.template operator()<i_>(std::get<i_>(combinators))); };

  index_over(std::index_sequence_for<T...>{});

  if (count == sizeof...(T))
    return result;
  else
  {
    ctx_.restore_state(backtrack);
    return std::nullopt;
  }
}

template <typename CONTEXT>
template <typename T>
auto combi_parse<CONTEXT>::one_or_more<T>::operator()(CONTEXT& ctx_) -> std::optional<return_type>
{
  using helper                                    = sequence<T, zero_or_more<T>>;
  std::optional<typename helper::return_type> tmp = helper{}(ctx_);

  if (!tmp.has_value())
    return std::nullopt;

  return_type result;
  result.reserve(std::get<1>(*tmp).size() + 1);

  result.push_back(std::move(std::get<0>(*tmp)));
  for (auto& elem : std::get<1>(*tmp))
    result.push_back(std::move(elem));
  return result;
}

template <typename CONTEXT>
template <typename T>
auto combi_parse<CONTEXT>::zero_or_more<T>::operator()(CONTEXT& ctx_) -> std::optional<return_type>
{
  return_type result;
  auto        backtrack = ctx_.capture_state();

  auto iterate = [&]() -> std::optional<typename T::return_type>
  {
    auto ret = T{}(ctx_);
    if (ret.has_value())
    {
      backtrack = ctx_.capture_state();
      return ret;
    }
    else
      return std::nullopt;
  };

  while (auto res = iterate())
    result.push_back(*res);

  ctx_.restore_state(backtrack);

  return result;
}

template <typename CONTEXT>
template <typename... T>
auto combi_parse<CONTEXT>::one_of<T...>::operator()(CONTEXT& ctx_) -> std::optional<return_type>
{
  std::optional<return_type> result;
  auto                       backtrack = ctx_.capture_state();

  auto fold_impl = [&]<std::size_t index_>(auto& parser_) -> bool
  {
    auto res = parser_(ctx_);
    if (res.has_value())
    {
      result = return_type{ std::in_place_index_t<index_>{}, std::move(*res) };
      return true;
    }
    else
      return false;
  };

  std::tuple<T...> combinators;
  auto             index_over = [&]<std::size_t... i_>(std::index_sequence<i_...>)
  { (... || fold_impl.template operator()<i_>(std::get<i_>(combinators))); };

  index_over(std::index_sequence_for<T...>{});

  if (!result.has_value())
    ctx_.restore_state(backtrack);

  return result;
}

template <typename CONTEXT>
template <typename T>
auto combi_parse<CONTEXT>::maybe<T>::operator()(CONTEXT& ctx_) -> std::optional<return_type>
{
  auto backtrack = ctx_.capture_state();
  auto res       = T{}(ctx_);
  if (res.has_value())
    return res;
  else
  {
    ctx_.restore_state(backtrack);
    return return_type{};
  }
}

template <typename CONTEXT>
template <typename T>
auto combi_parse<CONTEXT>::action<T>::operator()(CONTEXT& ctx_) -> std::optional<return_type>
{
  return T{}(ctx_) ? std::make_optional<return_type>() : std::nullopt;
}

template <typename CONTEXT>
template <typename DERIVED, typename OUT, typename T>
auto combi_parse<CONTEXT>::converter<DERIVED, OUT, T>::operator()(CONTEXT& ctx_) -> std::optional<return_type>
{
  if (auto res = T{}(ctx_); res.has_value())
    return static_cast<DERIVED&>(*this).convert(*res, ctx_);
  else
    return std::nullopt;
}

} // namespace pmt
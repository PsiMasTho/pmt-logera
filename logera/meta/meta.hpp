#pragma once

#include <utility>

namespace pmt
{

template <template <typename...> class OUTER, typename INNER, typename SEQ>
struct expander;

template <template <typename...> class OUTER, typename INNER, std::size_t... is_>
class expander<OUTER, INNER, std::index_sequence<is_...>>
{
  template <typename E, std::size_t>
  using helper = E;

public:
  using type = OUTER<helper<INNER, is_>...>;
};

template <template <typename...> class OUTER, typename INNER, std::size_t n_>
using expand_to_template = typename expander<OUTER, INNER, std::make_index_sequence<n_>>::type;

} // namespace pmt
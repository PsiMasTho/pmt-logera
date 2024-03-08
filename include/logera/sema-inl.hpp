// clang-format off
#ifdef __INTELLISENSE__
    #include "sema.hpp"
#endif
// clang-format on

#include <cassert>
#include <type_traits>

namespace sema
{

template <typename Derived>
pass_base<Derived>::pass_base(split_trees trees, error::container& errors)
    : m_trees{ trees }
    , m_errors{ errors }
{
    static_assert(std::is_base_of_v<pass_base<Derived>, Derived>);
    assert(std::get<ENTRY_ROOT_INDEX>(m_trees));
    assert(std::get<DECL_ATTR_ROOT_INDEX>(m_trees));
    assert(std::get<DECL_VAR_ROOT_INDEX>(m_trees));
}

template <typename Derived>
void pass_base<Derived>::operator()()
{
    static_cast<Derived*>(this)->run();
}

template <typename Derived>
auto pass_base<Derived>::entry_root() -> ast::multifile_node&
{
    return *std::get<ENTRY_ROOT_INDEX>(m_trees);
}

template <typename Derived>
auto pass_base<Derived>::decl_attr_root() -> ast::multifile_node&
{
    return *std::get<DECL_ATTR_ROOT_INDEX>(m_trees);
}

template <typename Derived>
auto pass_base<Derived>::decl_var_root() -> ast::multifile_node&
{
    return *std::get<DECL_VAR_ROOT_INDEX>(m_trees);
}

template <typename Derived>
auto pass_base<Derived>::errors() -> error::container&
{
    return m_errors;
}

template <typename... Ts>
auto apply_passes(split_trees trees, error::container& errors) -> bool
{
    auto const errors_before = errors.size();
    (Ts{ trees, errors }(), ...);
    return errors_before >= errors.size();
}

} // namespace sema

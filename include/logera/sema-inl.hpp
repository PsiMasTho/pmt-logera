// clang-format off
#ifdef __INTELLISENSE__
    #include "sema.hpp"
#endif
// clang-format on

namespace sema
{

template <pass... T> auto apply_passes(ast::multifile_node&& mf_node, std::vector<error::record>& errors) -> bool
{
    auto       trees         = std::make_tuple(std::move(mf_node), ast::multifile_node{}, ast::multifile_node{});
    auto       args          = std::tuple_cat(trees, std::make_tuple(std::ref(errors)));
    auto const errors_before = errors.size();
    (std::apply(T{}, args), ...);

    return errors_before >= errors.size();
}

} // namespace sema

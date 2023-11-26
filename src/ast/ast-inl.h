// clang-format off
#ifdef __INTELLISENSE__
    #include "ast.h"
#endif
// clang-format on

template <typename T>
auto ast<T>::add_node(T node) -> u32
{
    m_nodes.push_back(node);
    m_children.push_back({});
    return m_nodes.size() - 1;
}

template <typename T>
auto ast<T>::get_node(u32 idx) const -> T const&
{
    return m_nodes[idx];
}

template <typename T>
auto ast<T>::get_children(u32 idx) const -> std::vector<u32> const&
{
    return m_children[idx];
}

template <typename T>
auto ast<T>::add_child(u32 parent_idx, u32 child_idx) -> void
{
    m_children[parent_idx].push_back(child_idx);
}
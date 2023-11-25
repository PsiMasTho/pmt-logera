#pragma once

#include <vector>
#include <limits>
#include <algorithm>

#include "../type_aliases.h"

struct ast_node
{
    u32 offset;
    u8  type;
};

template <typename T>
class ast
{
    std::vector<T> m_nodes;
    std::vector<std::vector<u32>> m_children;
public:
    auto add_node(T node) -> u32;
    void add_child(u32 parent_idx, u32 child_idx);

    auto get_node(u32 idx) const -> T const&;
    auto get_children(u32 idx) const -> std::vector<u32> const&;

private:
    
};

#include "ast-inl.h"

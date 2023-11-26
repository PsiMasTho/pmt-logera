#pragma once

#include <vector>
#include <limits>
#include <algorithm>

#include "../type_aliases.h"

/*
    Each node is either a container or a leaf.
    A container has children, and its index is generally not interesting.
    A leaf has no children, and its index is the index into the lexed_file.
*/
struct ast_node
{
    u32 index; // *not* source_location, but index into lexed_file
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

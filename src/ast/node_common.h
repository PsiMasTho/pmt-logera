#pragma once

#include "../type_aliases.h"
#include <vector>

template <typename T>
struct container_node
{
    std::vector<T> children;
};

struct leaf_node
{
    u32 token_rec_idx; // index of the token_record in the lexed_buffer
};
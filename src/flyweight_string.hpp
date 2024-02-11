#pragma once

#include <memory>
#include <string_view>
#include <vector>

struct flyweight_string
{
    char const* data;

    using storage_type = std::vector<std::unique_ptr<char[]>>;

    flyweight_string() = default;
    flyweight_string(std::string_view const str, storage_type& storage);
};

//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <memory>
#include <string_view>
#include <vector>

class flyweight_string
{
public:
    using string_type  = std::pair<std::unique_ptr<char[]>, int>;
    using storage_type = std::vector<std::unique_ptr<string_type>>;

private:
    string_type* m_str;

public:
    flyweight_string() = default;
    flyweight_string(std::string_view const str, storage_type& storage);

    auto data() const -> char const*;
    auto size() const -> size_t;
};

auto to_string_view(flyweight_string const& s) -> std::string_view;

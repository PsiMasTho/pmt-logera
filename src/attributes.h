//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <functional>
#include <string>
#include <utility>

class attributes
{
    // Function to check a string against a bound regex
    using regex_match_checker = std::function<bool(std::string const&)>;
    using attribute = std::pair<std::string, std::vector<regex_match_checker>>;

    std::vector<attribute> m_attrs;

public:
    attributes();
    void add_attr(std::string const& name);
    void add_regex_to_last_attr(std::string const& expr);

    bool valid_value(size_t idx, std::string const& value) const;

    size_t get_count() const;
    size_t get_idx(std::string const& name) const;
    std::string const& get_name(size_t idx) const;
};
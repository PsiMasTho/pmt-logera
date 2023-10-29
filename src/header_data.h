//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "attributes.h"

#include <bitset>
#include <string>
#include <unordered_map>

enum
{
    MAX_ATTRIBUTES = 128
};

class header_data
{
    using var_to_attr_idx_map = std::unordered_map<std::string, std::bitset<MAX_ATTRIBUTES>>;

    // <var name, list of associated attribute indices from m_attrs>
    var_to_attr_idx_map m_vars;
    attributes m_attrs;
    var_to_attr_idx_map::iterator m_last_var_itr;

public:
    header_data();
    void add_var(std::string const& var_name);
    void add_attr(std::string const& attr_name);
    void add_regex_to_last_attr(std::string const& expr);
    void add_attr_to_last_var(std::string const& attr_name);

    bool does_var_have_attr(std::string const& var_name, std::string const& attr_name) const;

    attributes const& get_attributes() const;
};
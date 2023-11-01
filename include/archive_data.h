//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "date.h"
#include "detail/sparse_array.h"

#include <bitset>
#include <vector>
#include <string>
#include <unordered_map>

struct attribute_data
{
    std::string m_name;
    std::vector<std::string> m_reg_exprs;
};

struct header_data
{
    enum
    {
        MAX_ATTRIBUTES = 128
    };

    using var_to_attr_idx = std::unordered_map<std::string, std::bitset<MAX_ATTRIBUTES>>;

    // <var name, list of associated attribute indices from m_attrs>
    var_to_attr_idx m_vars;
    std::vector<attribute_data> m_attrs;
};

// class representing data of a single file
struct log_data
{
    using log_line = sparse_array<std::string>;

    date m_date;
    std::string m_filename;
    std::vector<log_line> m_lines;
};
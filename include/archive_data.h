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
#include <optional>

struct attribute_data
{
    std::string m_name;
    std::vector<std::string> m_reg_exprs;
};

struct variable_data
{
    std::string m_name;
    std::vector<bool> m_attr_indices;
};

struct header_data
{
    std::vector<attribute_data> m_attrs;
    std::vector<variable_data> m_vars;

    // assuming sorted data
    bool does_var_have_attr(std::string const& var_name, std::string const& attr_name) const;
    size_t get_attr_idx(std::string const& attr_name) const;
};

// class representing data of a single file
struct log_data
{
    using entry = sparse_array<std::string>;

    date m_date;
    std::string m_filename;
    std::vector<entry> m_entries;
};
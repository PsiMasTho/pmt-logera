//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "detail/sparse_array.h"
#include "log_date.h"

#include <string>
#include <vector>
#include <map>

struct attribute_data
{
    std::string name;
    std::vector<std::string> reg_exprs;
};

struct variable_data
{
    std::string name;
    std::vector<bool> attr_indices;
};

struct decl_data
{
    std::vector<attribute_data> attrs;
    std::vector<variable_data> vars;
};

struct entry_data
{
    std::string var_name;
    sparse_array<std::string> attr_values;
};

struct logera_data
{
    std::map<log_date, std::pair<std::string, std::vector<entry_data>>> entries; // <date, <filename, entries>>
    std::vector<decl_data> decls;
};
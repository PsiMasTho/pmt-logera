#pragma once

#include <string>
#include <vector>

struct attribute_data
{
    std::string              name;
    std::vector<std::string> reg_exprs;
};

struct variable_data
{
    std::string       name;
    std::vector<bool> attr_flags;
};

struct decl_data
{
    std::vector<attribute_data> attrs;
    std::vector<variable_data>  vars;
};

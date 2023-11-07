#include "parser_context_common.h"

#include "archive_data.h"

#include <algorithm>
#include <stdexcept>

using namespace std::string_literals;
using namespace std;

auto does_var_exist_lin(std::string const& var_name, header_data const& hd) -> bool
{
    auto const var_itr = find_if(hd.vars.begin(), hd.vars.end(),
        [&var_name](variable_data const& var)
        {
            return var.name == var_name;
        });

    return var_itr != hd.vars.end();
}

auto does_var_have_attr_lin(std::string const& var_name, std::string const& attr_name, header_data const& hd) -> bool
{
    auto const var_itr = find_if(hd.vars.begin(), hd.vars.end(),
        [&var_name](variable_data const& var)
        {
            return var.name == var_name;
        });

    if (var_itr == hd.vars.end())
        throw runtime_error("Unknown variable queried: "s + var_name);

    size_t const attr_idx = get_attr_idx_lin(attr_name, hd);
    return var_itr->attr_indices[attr_idx];
}

auto get_attr_idx_lin(std::string const& attr_name, header_data const& hd) -> std::size_t
{
    auto const attr_itr = find_if(hd.attrs.begin(), hd.attrs.end(),
        [&attr_name](attribute_data const& attr)
        {
            return attr.name == attr_name;
        });

    if (attr_itr == hd.attrs.end())
        throw runtime_error("Unknown attribute queried: "s + attr_name);

    return distance(hd.attrs.begin(), attr_itr);
}

auto get_var_idx_lin(std::string const& var_name, header_data const& hd) -> std::size_t
{
    auto const var_itr = find_if(hd.vars.begin(), hd.vars.end(),
        [&var_name](variable_data const& var)
        {
            return var.name == var_name;
        });

    if (var_itr == hd.vars.end())
        throw runtime_error("Unknown variable queried: "s + var_name);

    return distance(hd.vars.begin(), var_itr);
}

auto does_var_exist_bin(std::string const& var_name, header_data const& hd) -> bool
{
    auto const var_itr = lower_bound(hd.vars.begin(), hd.vars.end(), var_name,
        [](variable_data const& var, std::string const& var_name)
        {
            return var.name < var_name;
        });

    return var_itr != hd.vars.end() && var_itr->name == var_name;
}

auto does_var_have_attr_bin(std::string const& var_name, std::string const& attr_name, header_data const& hd) -> bool
{
    auto const var_itr = lower_bound(hd.vars.begin(), hd.vars.end(), var_name,
        [](variable_data const& var, std::string const& var_name)
        {
            return var.name < var_name;
        });
    
    if (var_itr == hd.vars.end() || var_itr->name != var_name)
        throw runtime_error("Unknown variable queried: "s + var_name);
    
    size_t const attr_idx = get_attr_idx_bin(attr_name, hd);

    return var_itr->attr_indices[attr_idx];
}

auto get_attr_idx_bin(std::string const& attr_name, header_data const& hd) -> std::size_t
{
    auto const attr_itr = lower_bound(hd.attrs.begin(), hd.attrs.end(), attr_name,
        [](attribute_data const& attr, std::string const& attr_name)
        {
            return attr.name < attr_name;
        });
    
    if (attr_itr == hd.attrs.end() || attr_itr->name != attr_name)
        throw runtime_error("Unknown attribute queried: "s + attr_name);
    
    return distance(hd.attrs.begin(), attr_itr);
}

auto get_var_idx_bin(std::string const& var_name, header_data const& hd) -> std::size_t
{
    auto const var_itr = lower_bound(hd.vars.begin(), hd.vars.end(), var_name,
        [](variable_data const& var, std::string const& var_name)
        {
            return var.name < var_name;
        });
    
    if (var_itr == hd.vars.end() || var_itr->name != var_name)
        throw runtime_error("Unknown variable queried: "s + var_name);
    
    return distance(hd.vars.begin(), var_itr);
}

auto does_var_have_attr_idx(std::size_t var_idx, std::size_t attr_idx, header_data const& hd) -> bool
{
    return hd.vars[var_idx].attr_indices[attr_idx];
}
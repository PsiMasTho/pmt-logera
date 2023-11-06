#include "header_parser_context.h"

#include <algorithm>
#include <stdexcept>
#include <numeric>

using namespace std;

header_parser_context::header_parser_context()
    : m_target{make_unique<header_data>()}
    , m_attr_name_hashes{}
    , m_var_name_hashes{}
{ }

void header_parser_context::add_var(string const& var_name)
{
    auto const var_name_hash = hash<string>{}(var_name);

    if (m_var_name_hashes.contains(var_name_hash))
        throw runtime_error("Variable name is not unique: "s + var_name);
    else
    {
        m_var_name_hashes.insert(var_name_hash);
        m_target->m_vars.emplace_back(variable_data{var_name, vector<bool>(m_target->m_attrs.size(), false)});
    }
}

void header_parser_context::add_attr(string const& attr_name)
{
    auto const attr_name_hash = hash<string>{}(attr_name);

    if (m_attr_name_hashes.contains(attr_name_hash))
        throw runtime_error("Attribute name is not unique: "s + attr_name);
    else
    {
        m_attr_name_hashes.insert(attr_name_hash);
        m_target->m_attrs.emplace_back(attribute_data{attr_name, {}});
    } 
}

void header_parser_context::add_regex_to_last_attr(string const& expr)
{
    if(m_target->m_attrs.empty())
        throw runtime_error("Trying to add regex without an attribute: "s + expr);
    
    // check if the regex is unique
    auto const& last_attr = m_target->m_attrs.back();
    if (any_of(last_attr.m_reg_exprs.begin(), last_attr.m_reg_exprs.end(),
        [&expr](string const& reg_expr)
        {
            return reg_expr == expr;
        }))
        throw runtime_error("Regex is not unique to attribute: "s + expr);

    m_target->m_attrs.back().m_reg_exprs.emplace_back(expr);
}

void header_parser_context::add_attr_to_last_var(string const& attr_name)
{
    auto const last_var_itr = get_last_var_itr();
    if (last_var_itr == m_target->m_vars.end())
        throw runtime_error("Attemtpting to add attribute without a variable: "s + attr_name);

    auto const attr_idx = get_attr_idx(attr_name);

    if (last_var_itr->m_attr_indices[attr_idx])
        throw runtime_error("Attempting to add attribute twice: "s + attr_name);

    last_var_itr->m_attr_indices[attr_idx] = true;
}

unique_ptr<header_data> header_parser_context::release_header_data()
{
    //sort_target_by_name();
    return move(m_target);
}

vector<variable_data>::iterator header_parser_context::get_last_var_itr()
{
    if (m_target->m_vars.empty())
        return m_target->m_vars.end();
    
    return prev(m_target->m_vars.end());
}

vector<attribute_data>::iterator header_parser_context::get_last_attr_itr()
{
    if (m_target->m_attrs.empty())
        return m_target->m_attrs.end();
    
    return prev(m_target->m_attrs.end());
}

void header_parser_context::sort_target_by_name()
{
    // sort variables
    sort(m_target->m_vars.begin(), m_target->m_vars.end(),
        [](variable_data const& lhs, variable_data const& rhs)
        {
            return lhs.m_name < rhs.m_name;
        });
    
    // get sorted indices of attributes
    vector<size_t> sorted_attr_indices(m_target->m_attrs.size());
    iota(sorted_attr_indices.begin(), sorted_attr_indices.end(), 0);
    sort(sorted_attr_indices.begin(), sorted_attr_indices.end(),
        [this](size_t lhs, size_t rhs)
        {
            return m_target->m_attrs[lhs].m_name < m_target->m_attrs[rhs].m_name;
        });
    
    // indirectly sort all variables' attribute indices
    for (auto& var : m_target->m_vars)
    {
        vector<bool> sorted_attr_indices(var.m_attr_indices.size());
        for (size_t i = 0; i < m_target->m_attrs.size(); ++i)
            sorted_attr_indices[i] = var.m_attr_indices[sorted_attr_indices[i]];
        var.m_attr_indices = move(sorted_attr_indices);
    }

    // sort attributes
    vector<attribute_data> sorted_attrs(m_target->m_attrs.size());
    for (size_t i = 0; i < m_target->m_attrs.size(); ++i)
        sorted_attrs[i] = move(m_target->m_attrs[sorted_attr_indices[i]]);
    m_target->m_attrs = move(sorted_attrs);
}

size_t header_parser_context::get_attr_idx(string const& attr_name) const
{
    auto const itr = find_if(m_target->m_attrs.begin(), m_target->m_attrs.end(),
        [&attr_name](attribute_data const& attr)
        {
            return attr.m_name == attr_name;
        });
    
    if (itr == m_target->m_attrs.end())
        throw runtime_error("Attribute not found: "s + attr_name);
    
    return distance(m_target->m_attrs.begin(), itr);
}
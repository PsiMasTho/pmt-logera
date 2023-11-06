#include "archive_data.h"

#include <algorithm>
#include <stdexcept>

using namespace std;

bool header_data::does_var_have_attr(std::string const& var_name, std::string const& attr_name) const
{
    auto const var_itr = find_if(m_vars.begin(), m_vars.end(),
        [&var_name](variable_data const& var)
        {
            return var.m_name == var_name;
        });

    if (var_itr == m_vars.end() || var_itr->m_name != var_name)
        throw runtime_error("Unknown variable queried: "s + var_name);

    size_t const attr_idx = get_attr_idx(attr_name);
    return var_itr->m_attr_indices[attr_idx];
}

size_t header_data::get_attr_idx(std::string const& attr_name) const
{
    auto const attr_itr = find_if(m_attrs.begin(), m_attrs.end(),
        [&attr_name](attribute_data const& attr)
        {
            return attr.m_name == attr_name;
        });

    if (attr_itr == m_attrs.end() || attr_itr->m_name != attr_name)
        throw runtime_error("Unknown attribute queried: "s + attr_name);

    return std::distance(m_attrs.begin(), attr_itr);
}

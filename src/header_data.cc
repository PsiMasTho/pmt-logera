//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "header_data.h"

#include <algorithm>
#include <stdexcept> // runtime_error

using namespace std;

header_data::header_data()
    : m_vars{}
    , m_attrs{}
    , m_last_var_itr{m_vars.end()}
{ }

void header_data::add_var(string const& var_name)
{
    auto const [itr, success] = m_vars.insert({var_name, {}});
    if(!success)
        throw runtime_error("Failed declaring variable: "s + var_name);
    else
        m_last_var_itr = itr;
}

void header_data::add_attr(string const& attr_name)
{
    m_attrs.add_attr(attr_name);
}

void header_data::add_regex_to_last_attr(std::string const& expr)
{
    m_attrs.add_regex_to_last_attr(expr);
}

void header_data::add_attr_to_last_var(std::string const& attr_name)
{
    if(m_last_var_itr == m_vars.end())
        throw runtime_error("Attemtpting to add attribute without a variable: "s + attr_name);

    m_last_var_itr->second[m_attrs.get_idx(attr_name)] = true;
}

bool header_data::does_var_have_attr(std::string const& var_name, std::string const& attr_name) const
{
    auto const itr = m_vars.find(var_name);
    if(itr == m_vars.end())
        throw runtime_error("Unknown variable queried: "s + var_name);
    else
        return itr->second[m_attrs.get_idx(attr_name)];
}

attributes const& header_data::get_attributes() const
{
    return m_attrs;
}
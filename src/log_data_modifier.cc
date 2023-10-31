//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "log_data_modifier.h"
#include "date.h"
#include "header_data.h"
#include "log_data.h"

#include <stdexcept>
#include <string>

using namespace std;

log_data_modifier::log_data_modifier(log_data* target, header_data const& hd)
    : m_target{target}
    , m_headerData{hd}
    , m_activeVar{}
{ }

void log_data_modifier::set_target(log_data* target)
{
    m_target = target;
}

void log_data_modifier::set_date(date const& date)
{
    m_target->m_date = date;
}

void log_data_modifier::set_filename(string const& filename)
{
    m_target->m_filename = filename;
}

void log_data_modifier::set_active_var(string const& var_name)
{
    m_activeVar = var_name;
}

void log_data_modifier::add_attr_to_new_line(string const& attr_name, string const& attr_val)
{
    if(m_activeVar.empty())
        throw runtime_error("Adding new line without active variable");

    m_target->m_lines.emplace_back(m_headerData.get_attributes().get_count() + 1);
    m_target->m_lines.back().set(0, m_activeVar);

    add_attr_to_current_line(attr_name, attr_val);
}

void log_data_modifier::add_attr_to_current_line(string const& attr_name, string const& attr_val)
{
    if(!m_target->m_lines.back().exists(0))
        throw runtime_error("Attempting to add attribute with no variable"s);

    if(!m_headerData.does_var_have_attr(m_activeVar, attr_name))
        throw runtime_error("Variable: \""s + m_activeVar + R"(" does not have attribute: ")"s +
                            attr_name + '\"');

    size_t const idx = m_headerData.get_attributes().get_idx(attr_name);
    if(!m_headerData.get_attributes().valid_value(idx, attr_val))
        throw runtime_error("Invalid value: \""s + attr_val + "\". For attribute: \"" + attr_name +
                            '\"');

    m_target->m_lines.back().set(idx + 1, attr_val);
}
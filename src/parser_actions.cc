//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "log_parser_actions.h"
#include "date.h"
#include "header_data.h"
#include "log_data.h"

#include <stdexcept>
#include <string>

using namespace std;

log_parser_actions::log_parser_actions(log_data* target, header_data const& hd)
    : m_target{target}
    , m_header_data{hd}
    , m_active_var{}
{ }

void log_parser_actions::set_target(log_data* target)
{
    m_target = target;
}

void log_parser_actions::set_date(date const& date)
{
    m_target->m_date = date;
}

void log_parser_actions::set_filename(string const& filename)
{
    m_target->m_filename = filename;
}

void log_parser_actions::set_active_var(string const& var_name)
{
    m_active_var = var_name;
}

void log_parser_actions::add_attr_to_new_line(string const& attr_name, string const& attr_val)
{
    if(m_active_var.empty())
        throw runtime_error("Adding new line without active variable");

    m_target->m_lines.emplace_back(m_header_data.get_attributes().get_count() + 1);
    m_target->m_lines.back().set(0, m_active_var);

    add_attr_to_current_line(attr_name, attr_val);
}

void log_parser_actions::add_attr_to_current_line(string const& attr_name, string const& attr_val)
{
    if(!m_target->m_lines.back().exists(0))
        throw runtime_error("Attempting to add attribute with no variable"s);

    if(!m_header_data.does_var_have_attr(m_active_var, attr_name))
        throw runtime_error("Variable: \""s + m_active_var + R"(" does not have attribute: ")"s +
                            attr_name + '\"');

    size_t const idx = m_header_data.get_attributes().get_idx(attr_name);
    if(!m_header_data.get_attributes().valid_value(idx, attr_val))
        throw runtime_error("Invalid value: \""s + attr_val + "\". For attribute: \"" + attr_name +
                            '\"');

    m_target->m_lines.back().set(idx + 1, attr_val);
}

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
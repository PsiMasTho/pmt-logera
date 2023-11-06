//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "log_parser_context.h"
#include "date.h"
#include "archive_data.h"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

void regex_matcher::add_regex(std::string const& expr)
{
    m_regexes.emplace_back(expr, regex::optimize);
}

bool regex_matcher::operator()(std::string const& str) const
{
    // if no regex is defined, then assume it's a match
    if (m_regexes.empty())
        return true;
    
    // does any regex match?
    return any_of(begin(m_regexes), end(m_regexes), [&str](auto const& rexpr) {
        return regex_match(str, rexpr);
    });
}

log_parser_context::log_parser_context(header_data * hd)
    : m_target{make_unique<log_data>()}
    , m_header_data{hd}
    , m_matchers{}
    , m_active_var{}
{
    construct_regexes();
}

void log_parser_context::set_date(date const& date)
{
    m_target->m_date = date;
}

void log_parser_context::set_filename(string const& filename)
{
    m_target->m_filename = filename;
}

void log_parser_context::set_active_var(string const& var_name)
{
    m_active_var = var_name;
}

void log_parser_context::add_attr_to_new_line(string const& attr_name, string const& attr_val)
{
    if(m_active_var.empty())
        throw runtime_error("Adding new line without active variable");

        // create a new line, allowing for one more column (the variable name)
    m_target->m_entries.emplace_back(m_header_data->m_attrs.size() + 1);
    m_target->m_entries.back().set(0, m_active_var);

    add_attr_to_current_line(attr_name, attr_val);
}

void log_parser_context::add_attr_to_current_line(string const& attr_name, string const& attr_val)
{
    if(!m_target->m_entries.back().exists(0))
        throw runtime_error("Attempting to add attribute with no variable"s);

    if(!m_header_data->does_var_have_attr(m_active_var, attr_name))
        throw runtime_error("Variable: \""s + m_active_var + R"(" does not have attribute: ")"s +
                            attr_name + '\"');

    size_t const idx = m_header_data->get_attr_idx(attr_name);
    if(!m_matchers[idx](attr_val))
        throw runtime_error("Invalid value: \""s + attr_val + "\". For attribute: \"" + attr_name +
                            '\"');

    m_target->m_entries.back().set(idx + 1, attr_val);
}

unique_ptr<log_data> log_parser_context::release_log_data()
{
    m_active_var.clear();
    return exchange(m_target, make_unique<log_data>());
}

void log_parser_context::construct_regexes()
{
    m_matchers.reserve(m_header_data->m_attrs.size());

    for (auto const& attr : m_header_data->m_attrs)
    {
        m_matchers.emplace_back();
        for (auto const& expr : attr.m_reg_exprs)
            m_matchers.back().add_regex(expr);
    }
}
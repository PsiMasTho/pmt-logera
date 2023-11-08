//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "log_parser_context.h"

#include "archive_data.h"
#include "log_date.h"
#include "parser_context_common.h"

#include <fmt/format.h>

#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;
using fmt::format;

namespace
{

[[noreturn]] void throw_var_doesnt_exist(std::string const& var_name)
{
    throw runtime_error(format("Variable: \"{}\" does not exist", var_name));
}

[[noreturn]] void throw_no_active_var()
{
    throw runtime_error("No active variable");
}

[[noreturn]] void throw_var_doesnt_have_attr(std::string const& var_name, std::string const& attr_name)
{
    throw runtime_error(format("Variable: \"{}\" does not have attribute: \"{}\"", var_name, attr_name));
}

[[noreturn]] void throw_attr_already_added(std::string const& attr_name)
{
    throw runtime_error(format("Attribute: \"{}\" already added", attr_name));
}

[[noreturn]] void throw_active_var_has_no_entries_at_eof(std::string const& var_name)
{
    throw runtime_error(format("End of file reached without any entries for: \"{}\"", var_name));
}

[[noreturn]] void throw_attr_val_regex_mismatch(std::string const& attr_name, std::string const& attr_val)
{
    throw runtime_error(format("Invalid value: \"{}\". For attribute: \"{}\"", attr_val, attr_name));
}

} // namespace

void regex_matcher::add_regex(std::string const& expr)
{
    m_regexes.emplace_back(expr, regex::optimize);
}

bool regex_matcher::operator()(std::string const& str) const
{
    // if no regex is defined, then assume it's a match
    if(m_regexes.empty())
        return true;

    // does any regex match?
    return any_of(begin(m_regexes), end(m_regexes), [&str](auto const& rexpr) { return regex_match(str, rexpr); });
}

log_parser_context::log_parser_context(header_data* hd)
    : m_target{make_unique<log_data>()}
    , m_header_data{hd}
    , m_matchers{}
    , m_active_variable_idx{}
    , m_entries_created_for_active_variable{0}
{
    construct_regexes();
}

void log_parser_context::set_date(log_date const& date)
{
    m_target->date = date;
}

void log_parser_context::set_filename(string const& filename)
{
    m_target->filename = filename;
}

void log_parser_context::set_active_variable_or_throw(std::string const& var_name)
{
    if(!does_var_exist_bin(var_name, *m_header_data))
        throw_var_doesnt_exist(var_name);

    m_active_variable_idx = get_var_idx_bin(var_name, *m_header_data);
    m_entries_created_for_active_variable = 0;
}

// 1. checks if active variable is set
// 2. checks if the set attributes belong to the active variable
void log_parser_context::create_entry_or_throw(sparse_array<std::string> const& attr_values)
{
    if(!m_active_variable_idx.has_value())
        throw_no_active_var();

    size_t const var_idx = m_active_variable_idx.value();
    for(size_t idx = 0; idx < m_header_data->attrs.size(); ++idx)
    {
        // skip if attribute is not set
        if(!attr_values.exists(idx))
            continue;

        if(!does_var_have_attr_idx(var_idx, idx, *m_header_data))
            throw_var_doesnt_have_attr(m_header_data->vars[var_idx].name, m_header_data->attrs[idx].name);
    }

    m_target->entries.emplace_back(m_header_data->vars[var_idx].name, attr_values);
    ++m_entries_created_for_active_variable;
}

auto log_parser_context::make_attr_value_arr_or_throw(std::pair<std::string, std::string> const& attr_value_pair)
    -> sparse_array<std::string>
{
    // validate regex
    size_t const attr_idx = get_attr_idx_bin(attr_value_pair.first, *m_header_data);
    validate_attr_val_regex_or_throw(attr_idx, attr_value_pair.second);

    // create sparse array
    sparse_array<std::string> ret(m_header_data->attrs.size());
    ret.set(attr_idx, attr_value_pair.second);
    return ret;
}

void log_parser_context::update_attr_value_arr_or_throw(sparse_array<std::string>& attr_value_arr,
                                                        std::pair<std::string, std::string> const& attr_value_pair)
{
    // validate regex
    size_t const attr_idx = get_attr_idx_bin(attr_value_pair.first, *m_header_data);
    validate_attr_val_regex_or_throw(attr_idx, attr_value_pair.second);

    // check if attribute already added before
    if(attr_value_arr.exists(attr_idx))
        throw_attr_already_added(attr_value_pair.first);

    // add to sparse array
    attr_value_arr.set(attr_idx, attr_value_pair.second);
}

// if there is an active variable pending but no entries then this is an error
void log_parser_context::at_eof()
{
    if(m_active_variable_idx.has_value() && m_entries_created_for_active_variable == 0)
        throw_active_var_has_no_entries_at_eof(m_header_data->vars[m_active_variable_idx.value()].name);
}

unique_ptr<log_data> log_parser_context::release_log_data()
{
    m_active_variable_idx.reset();
    m_entries_created_for_active_variable = 0;
    return exchange(m_target, make_unique<log_data>());
}

void log_parser_context::construct_regexes()
{
    m_matchers.reserve(m_header_data->attrs.size());

    for(auto const& attr : m_header_data->attrs)
    {
        m_matchers.emplace_back();
        for(auto const& expr : attr.reg_exprs)
            m_matchers.back().add_regex(expr);
    }
}

void log_parser_context::validate_attr_val_regex_or_throw(std::size_t attr_idx, std::string const& attr_val) const
{
    if(!m_matchers[attr_idx](attr_val))
        throw_attr_val_regex_mismatch(m_header_data->attrs[attr_idx].name, attr_val);
}
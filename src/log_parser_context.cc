//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "log_parser_context.h"

#include "archive_data.h"
#include "log_date.h"
#include "log_scanner.h"

#include <fmt/format.h>

#include <algorithm>
#include <stdexcept>
#include <string>
#include <filesystem>
#include <utility>

using namespace std;
using fmt::format;

void regex_matcher::add_regex(string const& expr)
{
    m_regexes.emplace_back(expr, regex::optimize);
}

bool regex_matcher::operator()(string const& str) const
{
    // if no regex is defined, then assume it's a match
    if(m_regexes.empty())
        return true;

    // does any regex match?
    return any_of(begin(m_regexes), end(m_regexes), [&str](auto const& rexpr) { return regex_match(str, rexpr); });
}

log_parser_context::log_parser_context(header_data const* hd)
    : m_target{make_unique<log_data>()}
    , m_header_data{hd}
    , m_scanner{nullptr}
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

void log_parser_context::set_scanner(log_scanner const& scanner)
{
    m_scanner = &scanner;
}

void log_parser_context::set_active_variable_or_err(string const& var_name)
{
    if(!does_var_exist_bin(var_name, *m_header_data))
        push_error(parse_error::SEMANTIC, m_scanner->filename(), format("Variable: \"{}\" does not exist", var_name), m_scanner->lineNr());

    m_active_variable_idx = get_var_idx_bin(var_name, *m_header_data);
    m_entries_created_for_active_variable = 0;
}

// 1. checks if active variable is set
// 2. checks if the set attributes belong to the active variable
void log_parser_context::create_entry_or_err(sparse_array<string> const& attr_values)
{
    if(!m_active_variable_idx.has_value())
        push_error(parse_error::SEMANTIC, m_scanner->filename(), "No active variable", m_scanner->lineNr());

    size_t const var_idx = m_active_variable_idx.value();
    for(size_t idx = 0; idx < m_header_data->attrs.size(); ++idx)
    {
        // skip if attribute is not set
        if(!attr_values.exists(idx))
            continue;

        if(!does_var_have_attr_idx(var_idx, idx, *m_header_data))
            push_error(parse_error::SEMANTIC, m_scanner->filename(), format("Variable: \"{}\" does not have attribute: \"{}\"", m_header_data->vars[var_idx].name, m_header_data->attrs[idx].name), m_scanner->lineNr());
    }

    m_target->entries.emplace_back(m_header_data->vars[var_idx].name, attr_values);
    ++m_entries_created_for_active_variable;
}

auto log_parser_context::make_attr_value_arr_or_err(pair<string, string> const& attr_value_pair)
    -> sparse_array<string>
{
    // validate regex
    size_t const attr_idx = get_attr_idx_bin(attr_value_pair.first, *m_header_data);
    validate_attr_val_regex_or_err(attr_idx, attr_value_pair.second);

    // create sparse array
    sparse_array<string> ret(m_header_data->attrs.size());
    ret.set(attr_idx, attr_value_pair.second);
    return ret;
}

void log_parser_context::update_attr_value_arr_or_err(sparse_array<string>& attr_value_arr,
                                                        pair<string, string> const& attr_value_pair)
{
    // validate regex
    size_t const attr_idx = get_attr_idx_bin(attr_value_pair.first, *m_header_data);
    validate_attr_val_regex_or_err(attr_idx, attr_value_pair.second);

    // check if attribute already added before
    if(attr_value_arr.exists(attr_idx))
        push_error(parse_error::SEMANTIC, m_scanner->filename(), format( "Attribute: \"{}\" already added", attr_value_pair.first), m_scanner->lineNr());

    // add to sparse array
    attr_value_arr.set(attr_idx, attr_value_pair.second);
}

// if there is an active variable pending but no entries then this is an error
void log_parser_context::at_eof()
{
    if(m_active_variable_idx.has_value() && m_entries_created_for_active_variable == 0)
        push_error(parse_error::SEMANTIC, m_scanner->filename(), format("End of file reached without any entries for: \"{}\"", m_header_data->vars[m_active_variable_idx.value()].name), m_scanner->lineNr());
}

unique_ptr<log_data> log_parser_context::release_log_data()
{
    set_filename_from_scanner();
    m_active_variable_idx.reset();
    m_entries_created_for_active_variable = 0;
    m_scanner = nullptr;
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

void log_parser_context::set_filename_from_scanner()
{
    if (!m_scanner)
        return;

    filesystem::path const filename_path(m_scanner->filename());
    m_target->filename = filename_path.filename().string();
}

void log_parser_context::validate_attr_val_regex_or_err(size_t attr_idx, string const& attr_val)
{
    if(!m_matchers[attr_idx](attr_val))
        push_error(parse_error::SEMANTIC, m_scanner->filename(), format("Invalid value: \"{}\". For attribute: \"{}\"", attr_val, m_header_data->attrs[attr_idx].name), m_scanner->lineNr());
}
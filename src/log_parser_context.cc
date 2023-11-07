//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "log_parser_context.h"

#include "archive_data.h"
#include "log_date.h"
#include "parser_context_common.h"

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

void log_parser_context::add_entry(entry_data const& entry)
{
    m_target->entries.push_back(entry);
}

auto log_parser_context::make_entry_or_throw(std::string const& var_name, sparse_array<std::string> const& attr_values) -> entry_data
{
    if (!does_var_exist_bin(var_name, *m_header_data))
        throw runtime_error("Variable: \""s + var_name + "\" does not exist"s);

    // check if variable has all attributes
    size_t const var_idx = get_var_idx_bin(var_name, *m_header_data);
    for (size_t idx = 0; idx < m_header_data->attrs.size(); ++idx)
    {
        if (!does_var_have_attr_idx(var_idx, idx, *m_header_data))
            throw runtime_error("Variable: \""s + var_name + R"(" does not have attribute: ")"s +
                                m_header_data->attrs[idx].name + '\"');
    }

    return entry_data{var_name, attr_values};
}

auto log_parser_context::make_attr_value_arr_or_throw(std::pair<std::string, std::string> const& attr_value_pair) -> sparse_array<std::string>
{
    // validate regex
    size_t const attr_idx = get_attr_idx_bin(attr_value_pair.first, *m_header_data);
    validate_attr_val_regex(attr_idx, attr_value_pair.second);

    // create sparse array
    sparse_array<std::string> ret(m_header_data->attrs.size());
    ret.set(attr_idx, attr_value_pair.second);
    return ret;
}

void log_parser_context::update_attr_value_arr_or_throw(sparse_array<std::string>& attr_value_arr, std::pair<std::string, std::string> const& attr_value_pair)
{
    // validate regex
    size_t const attr_idx = get_attr_idx_bin(attr_value_pair.first, *m_header_data);
    validate_attr_val_regex(attr_idx, attr_value_pair.second);

    // check if attribute already exists
    if (attr_value_arr.exists(attr_idx))
        throw runtime_error("Attribute: \""s + attr_value_pair.first + "\" already exists"s);
    
    // add to sparse array
    attr_value_arr.set(attr_idx, attr_value_pair.second);
}

unique_ptr<log_data> log_parser_context::release_log_data()
{
    return exchange(m_target, make_unique<log_data>());
}

void log_parser_context::construct_regexes()
{
    m_matchers.reserve(m_header_data->attrs.size());

    for (auto const& attr : m_header_data->attrs)
    {
        m_matchers.emplace_back();
        for (auto const& expr : attr.reg_exprs)
            m_matchers.back().add_regex(expr);
    }
}

auto log_parser_context::validate_attr_val_regex(std::size_t attr_idx, std::string const& attr_val) const -> bool
{
    if (!m_matchers[attr_idx](attr_val))
        throw runtime_error("Invalid value: \""s + attr_val + "\". For attribute: \"" +
                            m_header_data->attrs[attr_idx].name + '\"');
}
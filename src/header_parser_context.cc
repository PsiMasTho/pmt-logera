//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "header_parser_context.h"

#include "header_scanner.h"

#include <fmt/format.h>

#include <algorithm>
#include <filesystem>
#include <numeric>
#include <stdexcept>
#include <type_traits>

using namespace std;
using fmt::format;

namespace
{
template <typename T, typename U>
void indirect_sort(T data_begin, T data_end, U indices_begin)
{
    size_t const size = distance(data_begin, data_end);

    vector<typename T::value_type> tmp(size);
    transform(indices_begin, next(indices_begin, size), tmp.begin(), [&data_begin](size_t idx) { return *next(data_begin, idx); });

    // write back
    move(tmp.begin(), tmp.end(), data_begin);
}
} // namespace

header_parser_context::header_parser_context()
    : m_target{make_unique<header_data>()}
    , m_attr_name_hashes{}
    , m_var_name_hashes{}
    , m_scanner{nullptr}
{ }

void header_parser_context::set_scanner(header_scanner const& scanner)
{
    m_scanner = &scanner;
}

void header_parser_context::add_var(string const& var_name)
{
    auto const var_name_hash = hash<string>{}(var_name);

    if(m_var_name_hashes.contains(var_name_hash))
        push_error(parse_error::SEMANTIC, m_scanner->filename(), format("Variable name is not unique: {}", var_name), m_scanner->lineNr());
    else
    {
        m_var_name_hashes.insert(var_name_hash);
        m_target->vars.emplace_back(variable_data{var_name, vector<bool>(m_target->attrs.size(), false)});
    }
}

void header_parser_context::add_attr(string const& attr_name)
{
    auto const attr_name_hash = hash<string>{}(attr_name);

    if(m_attr_name_hashes.contains(attr_name_hash))
        push_error(
            parse_error::SEMANTIC, m_scanner->filename(), format("Attribute name is not unique: {}", attr_name), m_scanner->lineNr());
    else
    {
        m_attr_name_hashes.insert(attr_name_hash);
        m_target->attrs.emplace_back(attribute_data{attr_name, {}});
    }
}

void header_parser_context::add_regex_to_last_attr(string const& expr)
{
    if(m_target->attrs.empty())
        push_error(parse_error::SEMANTIC,
                   m_scanner->filename(),
                   format("Trying to add regex without an attribute: {}", expr),
                   m_scanner->lineNr());

    // check if the regex is unique
    auto const& last_attr = m_target->attrs.back();
    if(any_of(last_attr.reg_exprs.begin(), last_attr.reg_exprs.end(), [&expr](string const& reg_expr) { return reg_expr == expr; }))
        push_error(parse_error::SEMANTIC, m_scanner->filename(), format("Regex is not unique to attribute: {}", expr), m_scanner->lineNr());

    m_target->attrs.back().reg_exprs.emplace_back(expr);
}

void header_parser_context::add_attr_to_last_var(string const& attr_name)
{
    auto const last_var_itr = get_last_var_itr();
    if(last_var_itr == m_target->vars.end())
        push_error(parse_error::SEMANTIC,
                   m_scanner->filename(),
                   format("Trying to add attribute without a variable: {}", attr_name),
                   m_scanner->lineNr());

    auto const attr_idx = get_attr_idx_lin(attr_name, *m_target);

    if(last_var_itr->attr_indices[attr_idx])
        push_error(
            parse_error::SEMANTIC, m_scanner->filename(), format("Attepmting to add attribute twice: {}", attr_name), m_scanner->lineNr());

    last_var_itr->attr_indices[attr_idx] = true;
}

unique_ptr<header_data> header_parser_context::release_header_data()
{
    set_filename_from_scanner();
    sort_target_by_name();
    return move(m_target);
}

vector<variable_data>::iterator header_parser_context::get_last_var_itr()
{
    if(m_target->vars.empty())
        return m_target->vars.end();

    return prev(m_target->vars.end());
}

vector<attribute_data>::iterator header_parser_context::get_last_attr_itr()
{
    if(m_target->attrs.empty())
        return m_target->attrs.end();

    return prev(m_target->attrs.end());
}

void header_parser_context::set_filename_from_scanner()
{
    if(m_scanner == nullptr)
        return;

    filesystem::path const filename_path(m_scanner->filename());
    m_target->filename = filename_path.filename().string();
    ;
}

void header_parser_context::sort_target_by_name()
{
    // sort variables
    sort(m_target->vars.begin(), m_target->vars.end(), [](variable_data const& lhs, variable_data const& rhs) {
        return lhs.name < rhs.name;
    });

    // get sorted indices of attributes
    vector<size_t> sorted_attr_indices(m_target->attrs.size());
    iota(sorted_attr_indices.begin(), sorted_attr_indices.end(), 0);
    sort(sorted_attr_indices.begin(), sorted_attr_indices.end(), [this](size_t lhs, size_t rhs) {
        return m_target->attrs[lhs].name < m_target->attrs[rhs].name;
    });

    // indirectly sort all variables' attribute indices
    for(auto& var : m_target->vars)
        indirect_sort(begin(var.attr_indices), end(var.attr_indices), begin(sorted_attr_indices));

    // sort attributes
    indirect_sort(begin(m_target->attrs), end(m_target->attrs), begin(sorted_attr_indices));
}

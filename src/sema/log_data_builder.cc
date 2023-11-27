#include "log_data_builder.h"

#include "../lexer/lexed_buffer.h"
#include "../utility/utility.h"
#include "log_date.h"

#include <fmt/format.h>

using namespace std;

log_data_builder::log_data_builder(string const& filename,
                                   lexed_buffer const& lexed_buffer,
                                   header_data const& header,
                                   vector<regex_matcher> const& matchers)
    : m_filename(filename)
    , m_file(lexed_buffer)
    , m_result{}
    , m_errors{}
    , m_header(header)
    , m_matchers(matchers)
    , m_active_variable_idx{}
    , m_entries_created_for_active_variable{0}
{ }

void log_data_builder::operator()(log_node const& node)
{
    visit(*this, node);
}

void log_data_builder::operator()(log_root_node const& node)
{
    for(auto const& child : node.children)
        operator()(child);
}

void log_data_builder::operator()(log_statement_node const& node)
{
    for(auto const& child : node.children)
        operator()(child);
}

void log_data_builder::operator()(log_variable_node const& node)
{
    auto const ident_node = get<log_identifier_node>(node.children[0]);
    set_active_variable_or_err(m_file.get_token_record_at(ident_node.token_rec_idx));
}

void log_data_builder::operator()(log_ident_value_pair_list_node const& node)
{
    for(auto const& child : node.children)
    {
        auto const pair_node = get<log_ident_value_pair_node>(child);
        auto [attr_name, attr_val] = split_ident_value_pair(pair_node);
        auto const attr_idx = get_attr_idx(attr_name);

        if(!attr_idx)
        {
            auto const line_nr = m_file.get_line_nr_at(pair_node.token_rec_idx);
            push_error(fmt::format("Attribute: \"{}\" does not exist", attr_name), line_nr);
            continue;
        }

        if(!validate_attr_val_regex_or_err(*attr_idx, attr_val))
            continue;

        // todo: uniqueness check

        auto var_idx = *m_active_variable_idx;

        if(!does_var_have_attr_idx(var_idx, *attr_idx))
        {
            auto const line_nr = m_file.get_line_nr_at(pair_node.token_rec_idx);
            push_error(fmt::format("Variable: \"{}\" does not have attribute: \"{}\"", m_header.vars[var_idx].name, attr_name), line_nr);
            continue;
        }

        m_result.entries.emplace_back
    }
}

void log_data_builder::operator()(log_date_node const& node)
{
    auto const date_str = to_string(m_file.get_match_at(node.token_rec_idx));

    try
    {
        m_result.date = log_date(date_str);
    }
    catch(exception const& exc)
    {
        auto const line_nr = m_file.get_line_nr_at(node.token_rec_idx);
        push_error(exc.what(), line_nr);
    }
}

void log_data_builder::operator()(log_identifier_node const& node)
{
    // noop
}

void log_data_builder::operator()(log_ident_value_pair_node const& node)
{
    // noop
}

auto log_data_builder::release_result() -> log_data
{
    return std::move(m_result);
}

void log_data_builder::push_error(std::string const& msg, u32 line_nr)
{
    m_errors.emplace_back(parse_error::SEMANTIC, m_filename, msg, line_nr);
}

void log_data_builder::set_active_variable_or_err(token_record var)
{
    auto const var_name = to_string(m_file.get_match_at(var));

    if(!does_var_exist(var_name))
    {
        auto const line_nr = m_file.get_line_nr_at(var);
        push_error(fmt::format("Variable: \"{}\" does not exist", var_name), line_nr);
        return;
    }

    m_active_variable_idx = get_var_idx(var_name);
    m_entries_created_for_active_variable = 0;
}
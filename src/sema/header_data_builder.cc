#include "header_data_builder.h"

#include "../lexer/lexed_buffer.h"
#include "../utility/utility.h"

#include <fmt/format.h>

#include <algorithm>
#include <numeric>

using namespace std;

header_data_builder::header_data_builder(std::string const& filename, lexed_buffer const& lexed_buffer)
    : m_filename(filename)
    , m_file(lexed_buffer)
    , m_result{}
    , m_errors{}
{}

void header_data_builder::operator()(header_node const& node)
{
    visit(*this, node);
}

void header_data_builder::operator()(header_root_node const& node)
{
    m_result.filename = m_filename;

    for(auto const& child : node.children)
        operator()(child);

    if(!has_errors())
        sort_header_by_name();
}

void header_data_builder::operator()(header_statement_node const& node)
{
    for(auto const& child : node.children)
        operator()(child);
}

void header_data_builder::operator()(header_decl_var_node const& node)
{
    auto const var_record = m_file.get_token_record_at(get<header_identifier_node>(node.children.front()).token_rec_idx);

    // first child:
    if (!add_var_or_err(var_record))
        return;
    
    // all other children:
    for (auto const& child : node.children)
    {
        auto const attr_record = m_file.get_token_record_at(get<header_identifier_node>(child).token_rec_idx);

        if (!add_attr_to_last_var_or_err(attr_record))
            return;
    }
}

void header_data_builder::operator()(header_decl_attr_node const& node)
{
    auto const attr_record = m_file.get_token_record_at(get<header_identifier_node>(node.children.front()).token_rec_idx);

    // first child:
    if (!add_attr_or_err(attr_record))
        return;

    // all other children:
    for (auto const& child : node.children)
    {
        auto const expr_record = m_file.get_token_record_at(get<header_regex_node>(child).token_rec_idx);

        if (!add_regex_to_last_attr_or_err(expr_record))
            return;
    }
    
}

auto header_data_builder::release_result() -> header_data
{
    return std::move(m_result);
}

auto header_data_builder::get_errors() -> std::span<parse_error const>
{
    return m_errors;
}

auto header_data_builder::has_errors() const -> bool
{
    return !m_errors.empty();
}

void header_data_builder::push_error(std::string const& msg, u32 line_nr)
{
    m_errors.emplace_back(parse_error::SEMANTIC, m_filename, msg, line_nr);
}

auto header_data_builder::add_attr_or_err(token_record attr) -> bool
{
    // todo: checks
    m_result.attrs.emplace_back(attribute_data{to_string(m_file.get_match_at(attr)), {}});

    return true;
}

auto header_data_builder::add_var_or_err(token_record var) -> bool
{
    // todo: checks
    m_result.vars.emplace_back(variable_data{to_string(m_file.get_match_at(var)), vector<bool>(m_result.attrs.size(), false)});

    return true;
}

auto header_data_builder::add_regex_to_last_attr_or_err(token_record expr) -> bool
{
    // todo: checks
    m_result.attrs.back().reg_exprs.emplace_back(to_string(m_file.get_match_at(expr)));

    return true;
}

auto header_data_builder::add_attr_to_last_var_or_err(token_record attr) -> bool
{
    auto attr_name = to_string(m_file.get_match_at(attr));
    auto const attr_idx = get_attr_idx(attr_name);

    if (!attr_idx)
    {
        push_error(fmt::format("Attribute '{}' does not exist", attr_name), m_file.get_line_nr_at(attr));
        return false;
    }

    m_result.vars.back().attr_indices[*attr_idx] = true;

    return true;
}

auto header_data_builder::get_attr_idx(std::string const& attr_name) -> std::optional<u32>
{
    auto const attr_itr =
        find_if(m_result.attrs.begin(), m_result.attrs.end(), [&attr_name](attribute_data const& attr) { return attr.name == attr_name; });

    if(attr_itr == m_result.attrs.end())
        return nullopt;

    return distance(m_result.attrs.begin(), attr_itr);
}


void header_data_builder::sort_header_by_name()
{
    // sort variables
    sort(
        m_result.vars.begin(), m_result.vars.end(), [](variable_data const& lhs, variable_data const& rhs) { return lhs.name < rhs.name; });

    // get sorted indices of attributes
    vector<size_t> sorted_attr_indices(m_result.attrs.size());
    iota(sorted_attr_indices.begin(), sorted_attr_indices.end(), 0);
    sort(sorted_attr_indices.begin(), sorted_attr_indices.end(), [this](size_t lhs, size_t rhs) {
        return m_result.attrs[lhs].name < m_result.attrs[rhs].name;
    });

    // sort attributes
    auto cache = indirect_rearrange(m_result.attrs.begin(), m_result.attrs.end(), sorted_attr_indices.begin(), nullptr);

    // indirectly sort all variables' attribute indices
    for(auto& var : m_result.vars)
        cache = indirect_rearrange(begin(var.attr_indices), end(var.attr_indices), begin(sorted_attr_indices), move(cache));
}

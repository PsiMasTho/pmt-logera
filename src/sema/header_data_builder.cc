#include "header_data_builder.h"

#include "../ast/header_nodes.h"
#include "../lexer/lexed_buffer.h"

#include "../utility/utility.h"

#include <algorithm>
#include <numeric>

using namespace std;

header_data_builder::header_data_builder(string const& filename, lexed_buffer const& lexed_buffer)
    : m_filename(filename)
    , m_file(lexed_buffer)
    , m_result{}
    , m_errors{}
{ }

auto header_data_builder::pre(ast_node const& node) -> bool
{
    switch(static_cast<header_node_enum>(node.type))
    {
    case header_node_enum::ROOT:
        return pre_root(node.index);
    case header_node_enum::STATEMENT:
        return pre_statement(node.index);
    case header_node_enum::DECL_VAR:
        return pre_decl_var(node.index);
    case header_node_enum::DECL_ATTR:
        return pre_decl_attr(node.index);
    case header_node_enum::IDENTIFIER:
        return pre_identifier(node.index);
    case header_node_enum::REGEX:
        return pre_regex(node.index);
    }
}

auto header_data_builder::post(ast_node const& node) -> bool
{
    switch(static_cast<header_node_enum>(node.type))
    {
    case header_node_enum::ROOT:
        return post_root(node.index);
    case header_node_enum::STATEMENT:
        return post_statement(node.index);
    case header_node_enum::DECL_VAR:
        return post_decl_var(node.index);
    case header_node_enum::DECL_ATTR:
        return post_decl_attr(node.index);
    case header_node_enum::IDENTIFIER:
        return post_identifier(node.index);
    case header_node_enum::REGEX:
        return post_regex(node.index);
    }
}

auto header_data_builder::release_result() -> header_data
{
    return std::move(m_result);
}

auto header_data_builder::release_errors() -> std::vector<parse_error>
{
    return std::move(m_errors);
}

auto header_data_builder::has_errors() const -> bool
{
    return !m_errors.empty();
}

// ########################## pre ##########################
auto header_data_builder::pre_root(u32 idx) -> bool
{
    m_result.filename = m_filename;
}

auto header_data_builder::pre_statement(u32 idx) -> bool { }

auto header_data_builder::pre_decl_var(u32 idx) -> bool { }

auto header_data_builder::pre_decl_attr(u32 idx) -> bool { }

auto header_data_builder::pre_identifier(u32 idx) -> bool { }

auto header_data_builder::pre_regex(u32 idx) -> bool { }

// ########################## post ##########################
auto header_data_builder::post_root(u32 idx) -> bool
{
    sort_header_by_name();
}

auto header_data_builder::post_statement(u32 idx) -> bool { }

auto header_data_builder::post_decl_var(u32 idx) -> bool { }

auto header_data_builder::post_decl_attr(u32 idx) -> bool { }

auto header_data_builder::post_identifier(u32 idx) -> bool { }

auto header_data_builder::post_regex(u32 idx) -> bool { }

// ########################## helpers ##########################

void header_data_builder::push_error(std::string const& msg, u32 line_nr)
{
    m_errors.emplace_back(parse_error::SEMANTIC, m_filename, msg, line_nr);
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

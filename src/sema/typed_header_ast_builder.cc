#include "typed_header_ast_builder.h"

#include "../lexer/lexed_buffer.h"
#include "../utility/utility.h"

#include <algorithm>

using namespace std;

typed_header_ast_builder::typed_header_ast_builder(lexed_buffer const& lex)
    : m_lex(lex)
    , m_result{}
{ }

void typed_header_ast_builder::process(header_node const& node)
{
    process(get<header_root_node>(node));
}

auto typed_header_ast_builder::release_result() -> typed_header_ast
{
    return std::move(m_result);
}

void typed_header_ast_builder::process(header_root_node const& node)
{
    for(auto const& child : node.children)
    {
        auto result = process(get<header_statement_node>(child));

        if(holds_alternative<decl_attr_statement>(result))
            add_decl_attr_statement(std::move(get<decl_attr_statement>(result)));
        else if(holds_alternative<decl_var_statement>(result))
            add_decl_var_statement(std::move(get<decl_var_statement>(result)));
        else
            throw runtime_error("Unknown header statement type");
    }
}

auto typed_header_ast_builder::process(header_statement_node const& node) -> std::variant<decl_attr_statement, decl_var_statement>
{
    if(holds_alternative<header_decl_var_node>(node.children[0]))
        return process(get<header_decl_var_node>(node.children[0]));
    else if(holds_alternative<header_decl_attr_node>(node.children[0]))
        return process(get<header_decl_attr_node>(node.children[0]));
    else
        throw runtime_error("Unknown header statement type");
}

auto typed_header_ast_builder::process(header_decl_var_node const& node) -> decl_var_statement
{
    decl_var_statement result;
    result.variable = process(get<header_identifier_node>(node.children.front()));

    for(u32 i = 0; auto const& child : node.children)
    {
        if(i++ == 0)
            continue;

        result.attributes.push_back(process(get<header_identifier_node>(child)));
    }

    return result;
}

auto typed_header_ast_builder::process(header_decl_attr_node const& node) -> decl_attr_statement
{
    decl_attr_statement result;
    result.attribute = process(get<header_identifier_node>(node.children.front()));

    for(u32 i = 0; auto const& child : node.children)
    {
        if(i++ == 0)
            continue;

        result.expressions.push_back(process(get<header_regex_node>(child)));
    }

    return result;
}

auto typed_header_ast_builder::process(header_identifier_node const& node) -> tok_rec_idx_t
{
    return node.token_rec_idx;
}

auto typed_header_ast_builder::process(header_regex_node const& node) -> tok_rec_idx_t
{
    return node.token_rec_idx;
}

void typed_header_ast_builder::add_decl_attr_statement(decl_attr_statement&& statement)
{
    m_result.decl_attr_statements.push_back(std::move(statement));
}

void typed_header_ast_builder::add_decl_var_statement(decl_var_statement&& statement)
{
    m_result.decl_var_statements.push_back(std::move(statement));
}

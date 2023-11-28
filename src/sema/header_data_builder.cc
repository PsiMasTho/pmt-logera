#include "header_data_builder.h"

#include "typed_header_ast.h"
#include "../lexer/lexed_buffer.h"
#include "../utility/utility.h"

#include <algorithm>

using namespace std;

header_data_builder::header_data_builder(std::string const& filename, lexed_buffer const& lexed_buffer, typed_header_ast const& header_ast)
    : m_filename(filename)
    , m_lex(lexed_buffer)
    , m_header_ast(header_ast)
    , m_result{}
    , m_errors{}
{ }

void header_data_builder::process()
{
    m_result.filename = m_filename;

    for (auto const& statement : m_header_ast.decl_attr_statements)
        process_decl_attr_statement(statement);

    for (auto const& statement : m_header_ast.decl_var_statements)
        process_decl_var_statement(statement);
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

void header_data_builder::process_decl_attr_statement(decl_attr_statement const& statement)
{
    attribute_data result;
    result.name = m_lex.get_match_at(statement.attribute);

    for (auto expr_tok_rec_idx : statement.expressions)
        result.reg_exprs.push_back(to_string(m_lex.get_match_at(expr_tok_rec_idx)));
    
    m_result.attrs.push_back(std::move(result));
}

void header_data_builder::process_decl_var_statement(decl_var_statement const& statement)
{
    variable_data result;
    result.name = m_lex.get_match_at(statement.variable);
    result.attr_indices.resize(m_header_ast.decl_attr_statements.size(), false);

    for (auto attr_tok_rec_idx : statement.attributes)
    {
        string_view const attr_name = m_lex.get_match_at(attr_tok_rec_idx);
        auto const attr_idx = get_attr_idx(attr_name);
        if (attr_idx)
            result.attr_indices[*attr_idx] = true;
        else
            push_error("Attribute '" + to_string(attr_name) + "' is not declared", m_lex.get_line_nr_at(attr_tok_rec_idx));
    }

    m_result.vars.push_back(std::move(result));
}

void header_data_builder::push_error(std::string const& msg, u32 line_nr)
{
    m_errors.emplace_back(parse_error::SEMANTIC, m_filename, msg, line_nr);
}

auto header_data_builder::get_attr_idx(string_view attr_name) -> optional<u32>
{
    auto const attr_itr = lower_bound(begin(m_header_ast.decl_attr_statements), end(m_header_ast.decl_attr_statements), attr_name,
                                      [this](u32 attr, string_view name) { return m_lex.get_match_at(attr) < name; });
    
    if (attr_itr == end(m_header_ast.decl_attr_statements) || m_lex.get_match_at(attr_itr->attribute) != attr_name)
        return nullopt;
    
    return distance(begin(m_header_ast.decl_attr_statements), attr_itr);
}

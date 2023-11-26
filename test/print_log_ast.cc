/*
    This program is used to test the log parser. It takes a file as input and prints
    the AST.
*/

#include "../src/ast/ast.h"
#include "../src/ast/ast_visitor.h"
#include "../src/ast/log_nodes.h"
#include "../src/lexer/log_lexer.h"
#include "../src/parser/log_parser.h"
#include "../src/utility/utility.h"

#include <fmt/format.h>

#include <filesystem>
#include <iostream>
#include <string>

using namespace std;
namespace fs = std::filesystem;

class log_print_visitor
{
    lexed_buffer const& m_lexed_buffer;
    string m_indent = 0;

public:
    log_print_visitor(lexed_buffer const& lexed_buffer);

    auto pre(ast_node const& node) -> bool;
    auto post(ast_node const& node) -> bool;

private:
    void indent();
    void dedent();
};

log_print_visitor::log_print_visitor(lexed_buffer const& lexed_buffer)
    : m_lexed_buffer(lexed_buffer)
    , m_indent()
{ }

auto log_print_visitor::pre(ast_node const& node) -> bool
{
    switch(static_cast<log_node_enum>(node.type))
    {
    case log_node_enum::ROOT:
        fmt::print("{}", m_indent);
        fmt::print("root_node:\n");
        indent();
        break;
    case log_node_enum::STATEMENT:
        fmt::print("{}", m_indent);
        fmt::print("statement_node:\n");
        indent();
        break;
    case log_node_enum::VARIABLE:
        fmt::print("{}", m_indent);
        fmt::print("variable_node:\n");
        indent();
        break;
    case log_node_enum::IDENT_VALUE_PAIR_LIST:
        fmt::print("{}", m_indent);
        fmt::print("ident_value_pair_list_node: [\n");
        indent();
        break;
    case log_node_enum::DATE:
        fmt::print("{}", m_indent);
        fmt::print("date_node: {}\n", m_lexed_buffer.get_match_at(node.index));
        break;
    case log_node_enum::IDENTIFIER:
        fmt::print("{}", m_indent);
        fmt::print("identifier_node: {}\n", m_lexed_buffer.get_match_at(node.index));
        break;
    case log_node_enum::IDENT_VALUE_PAIR:
        fmt::print("{}", m_indent);
        fmt::print("ident_value_pair_node: {}\n", m_lexed_buffer.get_match_at(node.index));
        break;
    }

    return true;
}

auto log_print_visitor::post(ast_node const& node) -> bool
{
    switch(static_cast<log_node_enum>(node.type))
    {
    case log_node_enum::ROOT:
        dedent();
        break;
    case log_node_enum::STATEMENT:
        dedent();
        break;
    case log_node_enum::VARIABLE:
        dedent();
        break;
    case log_node_enum::IDENT_VALUE_PAIR_LIST:
        fmt::print("{}", m_indent);
        fmt::print("]\n");
        dedent();
        break;
    default:
        break;
    }

    return true;
}

void log_print_visitor::indent()
{
    m_indent += "    ";
}

void log_print_visitor::dedent()
{
    if(m_indent.size() >= 4)
        m_indent = m_indent.substr(0, m_indent.size() - 4);
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        fmt::print("Usage: {} <file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if(!filesystem::exists(argv[1]))
    {
        fmt::print("File does not exist: {}\n", argv[1]);
        return 1;
    }

    auto buffer = read_file(argv[1]);

    if(!buffer.first)
    {
        fmt::print("Error reading file: {}\n", argv[1]);
        return 1;
    }

    log_lexer lexer(std::move(buffer));

    if(lexer.lex() != EXIT_SUCCESS)
    {
        fmt::print("Error lexing file: {}\n", argv[1]);
        return 1;
    }

    lexed_buffer lex_result = lexer.release_result();

    log_parser parser(lex_result);

    auto ast = parser.gen();

    if(!ast)
    {
        fmt::print("Error parsing file: {}\n", argv[1]);
        return EXIT_FAILURE;
    }

    ast_visitor visitor(ast.value(), log_print_visitor(lex_result));
    visitor.visit(0);
}
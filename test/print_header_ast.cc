/*
    This program is used to test the header parser. It takes a file as input and prints
    the AST.
*/

#include "../src/ast/ast.h"
#include "../src/ast/ast_visitor.h"
#include "../src/ast/header_nodes.h"
#include "../src/lexer/header_lexer.h"
#include "../src/parser/header_parser.h"
#include "../src/utility/utility.h"

#include <fmt/format.h>

#include <filesystem>
#include <iostream>
#include <string>

using namespace std;
namespace fs = std::filesystem;

class header_print_visitor
{
    lexed_buffer const& m_lexed_buffer;
    string m_indent = 0;

public:
    header_print_visitor(lexed_buffer const& lexed_buffer);

    auto pre(ast_node const& node) -> bool;
    auto post(ast_node const& node) -> bool;

private:
    void indent();
    void dedent();
};

header_print_visitor::header_print_visitor(lexed_buffer const& lexed_buffer)
    : m_lexed_buffer(lexed_buffer)
    , m_indent()
{ }

auto header_print_visitor::pre(ast_node const& node) -> bool
{
    switch(static_cast<header_node_enum>(node.type))
    {
    case header_node_enum::ROOT:
        fmt::print("{}", m_indent);
        fmt::print("root_node:\n");
        indent();
        break;
    case header_node_enum::STATEMENT:
        fmt::print("{}", m_indent);
        fmt::print("statement_node:\n");
        indent();
        break;
    case header_node_enum::IDENTIFIER:
        fmt::print("{}", m_indent);
        fmt::print("identifier_node: {}\n", m_lexed_buffer.get_match_at(node.index));
        break;
    case header_node_enum::REGEX:
        fmt::print("{}", m_indent);
        fmt::print("regex_node: {}\n", m_lexed_buffer.get_match_at(node.index));
        break;
    case header_node_enum::DECL_VAR:
        fmt::print("{}", m_indent);
        fmt::print("decl_var_node: [\n");
        indent();
        break;
    case header_node_enum::DECL_ATTR:
        fmt::print("{}", m_indent);
        fmt::print("decl_attr_node: [\n");
        indent();
        break;
    }

    return true;
}

auto header_print_visitor::post(ast_node const& node) -> bool
{
    switch(static_cast<header_node_enum>(node.type))
    {
    case header_node_enum::ROOT:
        dedent();
        break;
    case header_node_enum::STATEMENT:
        dedent();
    case header_node_enum::DECL_VAR:
        fmt::print("{}", m_indent);
        fmt::print("]\n");
        dedent();
        break;
    case header_node_enum::DECL_ATTR:
        fmt::print("{}", m_indent);
        fmt::print("]\n");
        dedent();
        break;
    default:
        break;
    }

    return true;
}

void header_print_visitor::indent()
{
    m_indent += "    ";
}

void header_print_visitor::dedent()
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

    header_lexer lexer(std::move(buffer));

    if(lexer.lex() != EXIT_SUCCESS)
    {
        fmt::print("Error lexing file: {}\n", argv[1]);
        return 1;
    }

    lexed_buffer lex_result = lexer.release_result();

    header_parser parser(lex_result);

    auto ast = parser.gen();

    if(!ast)
    {
        fmt::print("Error parsing file: {}\n", argv[1]);
        return EXIT_FAILURE;
    }

    ast_visitor visitor(ast.value(), header_print_visitor(lex_result));
    visitor.visit();
}
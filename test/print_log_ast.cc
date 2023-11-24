/*
    This program is used to test the log parser. It takes a file as input and prints
    the AST.
*/

#include "../src/lexer/log_lexer.h"
#include "../src/parser/log_parser.h"

#include <fmt/format.h>

#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void print_ast(date_node const& node, lexed_file const& file, std::string indent)
{
    fmt::print("{}date_node: {}\n", indent, file.get_match_at(node.m_date));
}

void print_ast(variable_node const& node, lexed_file const& file, std::string indent)
{
    fmt::print("{}variable_node: {}\n", indent, file.get_match_at(node.m_identifier));
}

void print_ast(newline_node const& node, lexed_file const& file, std::string indent)
{
    fmt::print("{}newline_node: *nl*\n", indent);
}

void print_ast(eof_node const& node, lexed_file const& file, std::string indent)
{
    fmt::print("{}eof_node: *eof*\n", indent);
}

void print_ast(ident_value_pair_node const& node, lexed_file const& file , std::string indent)
{
    fmt::print("{}ident_value_pair_node: {{{}}}\n", indent, file.get_match_at(node.m_pair));
}

void print_ast(ident_value_pair_list_node const& node, lexed_file const& file, std::string indent)
{
    fmt::print("ident_value_pair_list_node: [");

    indent.append("    ");
    for (auto const& pair : node.m_pairs)
        print_ast(pair, file, indent);
    indent.erase(indent.size() - 4);

    fmt::print("]\n");
}

void print_ast(log_file_ast const& ast, lexed_file const& file, std::string indent = "")
{
    for (auto const& node : ast.m_nodes)
    {
        std::visit(
            [&](auto const& node)
            {
                print_ast(node, file, indent);
            },
            node);
    }
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fmt::print("Usage: {} <file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!filesystem::exists(argv[1]))
    {
        fmt::print("File does not exist: {}\n", argv[1]);
        return 1;
    }

    log_lexer lexer(argv[1]);

    if (lexer.lex() != EXIT_SUCCESS)
    {
        fmt::print("Error lexing file: {}\n", argv[1]);
        return 1;
    }

    lexed_file lex_result = lexer.release_result();

    log_parser parser(lex_result, 0);

    auto ast = parser.gen();

    if (!ast)
    {
        fmt::print("Error parsing file: {}\n", argv[1]);
        return EXIT_FAILURE;
    }

    print_ast(*ast, lex_result);
}
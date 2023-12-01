/*
    This program is used to test the log parser. It takes a file as input and prints
    the AST.
*/

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

// struct log_root_node,
//   struct log_statement_node,
//   struct log_variable_node,
//   struct log_ident_value_pair_list_node,
//   struct log_ident_value_pair_node,
//   struct log_date_node,
//   struct log_attr_value_node,
//   struct log_identifier_node

class log_printer
{
    lexed_buffer const& m_lexed_buffer;
    string m_indent = 0;

public:
    log_printer(lexed_buffer const& lexed_buffer);

    void operator()(log_node const& node);

    void operator()(log_root_node const& node);
    void operator()(log_statement_node const& node);
    void operator()(log_variable_node const& node);
    void operator()(log_ident_value_pair_list_node const& node);
    void operator()(log_ident_value_pair_node const& node);
    void operator()(log_date_node const& node);
    void operator()(log_attr_value_node const& node);
    void operator()(log_identifier_node const& node);

private:
    void indent();
    void dedent();
};

log_printer::log_printer(lexed_buffer const& lexed_buffer)
    : m_lexed_buffer(lexed_buffer)
    , m_indent()
{ }

void log_printer::operator()(log_node const& node)
{
    visit(*this, node);
}

void log_printer::operator()(log_root_node const& node)
{
    fmt::print("{}", m_indent);
    fmt::print("root_node:\n");
    indent();
    for(auto const& child : node.children)
        operator()(child);
    dedent();
}

void log_printer::operator()(log_statement_node const& node)
{
    fmt::print("{}", m_indent);
    fmt::print("statement_node: [\n");

    indent();
    for(auto const& child : node.children)
        operator()(child);
    dedent();

    fmt::print("{}", m_indent);
    fmt::print("]\n");
}

void log_printer::operator()(log_variable_node const& node)
{
    fmt::print("{}", m_indent);
    fmt::print("variable_node:\n");
    indent();
    for(auto const& child : node.children)
        operator()(child);
    dedent();
}

void log_printer::operator()(log_ident_value_pair_list_node const& node)
{
    fmt::print("{}", m_indent);
    fmt::print("ident_value_pair_list_node: [\n");

    indent();
    for(auto const& child : node.children)
        operator()(child);
    dedent();

    fmt::print("{}", m_indent);
    fmt::print("]\n");
}

void log_printer::operator()(log_ident_value_pair_node const& node)
{
    fmt::print("{}", m_indent);
    fmt::print("ident_value_pair_node: [\n");

    indent();
    for(auto const& child : node.children)
        operator()(child);
    dedent();

    fmt::print("{}", m_indent);
    fmt::print("]\n");
}

void log_printer::operator()(log_date_node const& node)
{
    fmt::print("{}", m_indent);
    fmt::print("date_node: {}\n", m_lexed_buffer.get_match_at(node.token_rec_idx));
}

void log_printer::operator()(log_attr_value_node const& node)
{
    fmt::print("{}", m_indent);
    fmt::print("attr_value_node: {}\n", m_lexed_buffer.get_match_at(node.token_rec_idx));
}

void log_printer::operator()(log_identifier_node const& node)
{
    fmt::print("{}", m_indent);
    fmt::print("identifier_node: {}\n", m_lexed_buffer.get_match_at(node.token_rec_idx));
}

void log_printer::indent()
{
    m_indent += "    ";
}

void log_printer::dedent()
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

    auto ast = parser.release_result();

    log_printer printer(lex_result);
    printer(ast);
}
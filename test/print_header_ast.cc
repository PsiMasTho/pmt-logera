/*
    This program is used to test the log parser. It takes a file as input and prints
    the AST.
*/

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

class header_printer
{
    lexed_buffer const& m_lexed_buffer;
    string m_indent = 0;

public:
    header_printer(lexed_buffer const& lexed_buffer);

    void operator()(header_node const& node);

    void operator()(header_root_node const& node);
    void operator()(header_statement_node const& node);
    void operator()(header_decl_var_node const& node);
    void operator()(header_decl_attr_node const& node);
    void operator()(header_identifier_node const& node); // leaf
    void operator()(header_regex_node const& node); // leaf

private:
    void indent();
    void dedent();
};

header_printer::header_printer(lexed_buffer const& lexed_buffer)
    : m_lexed_buffer(lexed_buffer)
    , m_indent()
{ }

void header_printer::operator()(header_node const& node)
{
    visit(*this, node);
}

void header_printer::operator()(header_root_node const& node)
{
    fmt::print("{}", m_indent);
    fmt::print("root_node:\n");
    indent();
    for(auto const& child : node.children)
        operator()(child);
    dedent();
}

void header_printer::operator()(header_statement_node const& node)
{
    fmt::print("{}", m_indent);
    fmt::print("statement_node:\n");
    indent();
    for(auto const& child : node.children)
        operator()(child);
    dedent();
}

void header_printer::operator()(header_decl_var_node const& node)
{
    fmt::print("{}", m_indent);
    fmt::print("decl_attr_node: [\n");

    indent();
    for(auto const& child : node.children)
        operator()(child);
    dedent();

    fmt::print("{}", m_indent);
    fmt::print("]\n");
}

void header_printer::operator()(header_decl_attr_node const& node)
{
    fmt::print("{}", m_indent);
    fmt::print("decl_attr_node: [\n");

    indent();
    for(auto const& child : node.children)
        operator()(child);
    dedent();

    fmt::print("{}", m_indent);
    fmt::print("]\n");
}

void header_printer::operator()(header_identifier_node const& node)
{
    fmt::print("{}", m_indent);
    fmt::print("identifier_node: {}\n", m_lexed_buffer.get_match_at(node.token_rec_idx));
}

void header_printer::operator()(header_regex_node const& node)
{
    fmt::print("{}", m_indent);
    fmt::print("regex_node: {}\n", m_lexed_buffer.get_match_at(node.token_rec_idx));
}

void header_printer::indent()
{
    m_indent += "    ";
}

void header_printer::dedent()
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

    auto ast = parser.release_result();

    header_printer printer(lex_result);
    printer(ast);
}
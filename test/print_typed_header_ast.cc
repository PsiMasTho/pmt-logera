#include "../src/lexer/header_lexer.h"
#include "../src/parser/header_parser.h"
#include "../src/sema/typed_header_ast_builder.h"
#include "../src/utility/utility.h"

#include <fmt/format.h>

#include <filesystem>
#include <iostream>
#include <string>

using namespace std;
namespace fs = std::filesystem;

void print_typed_header_ast(typed_header_ast const& ast, lexed_buffer const& lex)
{
    fmt::print("Decl attr statements:\n");
    for (auto const& statement : ast.decl_attr_statements)
    {
        fmt::print("    attribute: {}\n", lex.get_match_at(statement.attribute));

        for (auto const& expression : statement.expressions)
            fmt::print("        expression: {}\n", lex.get_match_at(expression));
    }

    fmt::print("\nDecl var statements:\n");
    for (auto const& statement : ast.decl_var_statements)
    {
        fmt::print("    variable: {}\n", lex.get_match_at(statement.variable));

        for (auto const& attribute : statement.attributes)
            fmt::print("        attribute: {}\n", lex.get_match_at(attribute));
    }
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

    auto typed_ast = [&] {
        typed_header_ast_builder builder(lex_result);
        builder.process(ast);
        return builder.release_result();
    }();

    print_typed_header_ast(typed_ast, lex_result);
}
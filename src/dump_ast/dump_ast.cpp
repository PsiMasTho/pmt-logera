#include "logera/ast.hpp"
#include "logera/errors.hpp"
#include "logera/io.hpp"
#include "logera/lexer.hpp"
#include "logera/overloaded.hpp"
#include "logera/parser.hpp"
#include "logera/sema.hpp"

#include <algorithm>
#include <cstdio>
#include <functional>
#include <string>
#include <vector>

using namespace std;

int       cur_indent  = 0;
int const indent_incr = 4;

void      indent()
{
    cur_indent += indent_incr;
}

void dedent()
{
    cur_indent -= indent_incr;
}

void print_indent()
{
    for (int i = 0; i < cur_indent; ++i)
        printf(" ");
}

void print_ast(ast::multifile_node const& multifile)
{
    using namespace ast;
    auto printer = overloaded{
        [](auto const* self, multifile_node const& n)
        {
            print_indent();
            printf("multifile_node:\n");
            indent();
            print_indent();
            printf("- children [\n");
            indent();
            for (auto const& file : n.children)
                invoke(*self, self, file);
            dedent();
            print_indent();
            printf("]\n");
            dedent();
        },
        [](auto const* self, file_node const& n)
        {
            print_indent();
            printf("file_node:\n");
            indent();
            print_indent();
            printf("- filename: %s\n", n.filename.c_str());
            print_indent();
            printf("- children [\n");
            indent();
            for (auto const& child : n.children)
                visit([&](auto const& c) { invoke(*self, self, c); }, child);
            dedent();
            print_indent();
            printf("]\n");
            dedent();
        },
        [](auto const* self, decl_attr_node const& n)
        {
            print_indent();
            printf("decl_attr_node:\n");
            indent();
            print_indent();
            printf("- identifier: %s\n", n.identifier.record.lexeme.data());
            print_indent();
            printf("- children [\n");
            indent();
            for (auto const& child : n.children)
                invoke(*self, self, child);
            dedent();
            print_indent();
            printf("]\n");
            dedent();
        },
        [](auto const* self, decl_var_node const& n)
        {
            print_indent();
            printf("decl_var_node:\n");
            indent();
            print_indent();
            printf("- identifier: %s\n", n.identifier.record.lexeme.data());
            print_indent();
            printf("- children [\n");
            indent();
            for (auto const& child : n.children)
                invoke(*self, self, child);
            dedent();
            print_indent();
            printf("]\n");
            dedent();
        },
        [](auto const*, ident_value_pair_node const& n)
        {
            print_indent();
            printf("ident_value_pair_node:\n");
            indent();
            print_indent();
            printf("- identifier: %s\n", n.identifier.record.lexeme.data());
            print_indent();
            printf("- attr_value: %s\n", n.attr_value.record.lexeme.data());
            dedent();
        },
        [](auto const* self, ident_value_pair_list_node const& n)
        {
            print_indent();
            printf("ident_value_pair_list_node:\n");
            indent();
            print_indent();
            printf("- children [\n");
            indent();
            for (auto const& child : n.children)
                invoke(*self, self, child);
            dedent();
            print_indent();
            printf("]\n");
            dedent();
        },
        [](auto const* self, entry_node const& n)
        {
            print_indent();
            printf("entry_node:\n");
            indent();
            print_indent();
            printf("- identifier: %s\n", n.identifier.record.lexeme.data());
            print_indent();
            printf("- ident_value_pair_list:\n");
            invoke(*self, self, n.ident_value_pair_list);
            dedent();
        },
        [](auto const*, date_node const& n)
        {
            print_indent();
            printf("date_node:\n");
            indent();
            print_indent();
            printf("- record: %s\n", n.record.lexeme.data());
            print_indent();
            printf("- location: (l:%d, c:%d)\n", n.record.location.line, n.record.location.column);
            dedent();
        },
        [](auto const*, regex_node const& n)
        {
            print_indent();
            printf("regex_node:\n");
            indent();
            print_indent();
            printf("- record: %s\n", n.record.lexeme.data());
            print_indent();
            printf("- location: (l:%d, c:%d)\n", n.record.location.line, n.record.location.column);
            dedent();
        },
        [](auto const*, identifier_node const& n)
        {
            print_indent();
            printf("identifier_node:\n");
            indent();
            print_indent();
            printf("- record: %s\n", n.record.lexeme.data());
            print_indent();
            printf("- location: (l:%d, c:%d)\n", n.record.location.line, n.record.location.column);
            dedent();
        },
    };

    printer(&printer, multifile);
}

void print_error(error::record const& e)
{
    printf("Error: %s\n", e.msg.c_str());
}

auto process_file(char const* filename, lexer& l, string& buffer, flyweight_string::storage_type& storage)
    -> ast::file_node
{
    vector<error::record> errors;

    io::readallf(filename, buffer, errors);
    if (!errors.empty())
    {
        for (auto const& e : errors)
            print_error(e);
        return {};
    }

    // add an extra newline for the lexer and parser
    buffer.push_back('\n');

    l.set_buffer(filename, buffer.data(), buffer.size());

    parser p(l, storage, errors);
    auto   file = p.parse();

    if (!errors.empty())
        for (auto const& e : errors)
            print_error(e);

    return file;
}

void process_files(char** filenames, int num_files)
{
    flyweight_string::storage_type storage;
    string                         buffer;
    lexer                          l;
    ast::multifile_node            multifile;
    for (int i = 0; i < num_files; ++i)
    {
        ast::file_node file = process_file(filenames[i], l, buffer, storage);
        if (!file.children.empty())
            multifile.children.push_back(std::move(file));
    }

    vector<error::record> errors;
    sema::checker         checker(std::move(multifile), errors);
    checker.check();

    if (!errors.empty())
        for (auto const& e : errors)
            print_error(e);
    errors.clear();

    printf("Attributes:\n");
    printf("-----------\n");
    if (!checker.get_decl_attr_root().children.empty())
        print_ast(checker.get_decl_attr_root());

    printf("Variables:\n");
    printf("-----------\n");
    if (!checker.get_decl_var_root().children.empty())
        print_ast(checker.get_decl_var_root());

    printf("Main tree:\n");
    printf("-----------\n");
    if (!checker.get_entry_root().children.empty())
        print_ast(checker.get_entry_root());
}

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        fprintf(stderr, "Usage: %s <file> [<file> ...]\n", argv[0]);
        return 1;
    }

    ++argv;
    --argc;

    // make the output easier to look through
    sort(argv, argv + argc);

    process_files(argv, argc);
}
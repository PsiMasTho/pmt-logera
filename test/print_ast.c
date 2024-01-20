#include "../lib/io.h"
#include "../lib/ast.h"
#include "../lib/lexer.h"
#include "../lib/parser.h"
#include "../lib/errors.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cur_indent = 0;
int const indent_incr = 4;

void indent(void)
{
    cur_indent += indent_incr;
}

void dedent(void)
{
    cur_indent -= indent_incr;
}

void print_indent(void)
{
    for (int i = 0; i < cur_indent; ++i)
        printf(" ");
}

void print_ast(ast_node* node)
{
    print_indent();
    printf("%s:\n", ast_node_type_properties_table[node->type].name);
    indent();

    if (ast_node_type_properties_table[node->type].has_tok)
    {
        print_indent();
        printf("- lexeme: %s, (l %d, c %d)\n", node->tok.lexeme, node->tok.loc.line, node->tok.loc.column);
    }

    if (ast_node_type_properties_table[node->type].has_unowned_str)
    {
        print_indent();
        printf("- str: %s\n", node->unowned_str);
    }

    if (ast_node_type_properties_table[node->type].has_owned_str)
    {
        print_indent();
        printf("- str: %s\n", node->owned_str);
    }
    
    if (ast_node_type_properties_table[node->type].has_children)
    {
        print_indent();
        printf("- children [\n");
        indent();
        for (ast_node* child = node->children.begin; child != node->children.end; ++child)
        {
            assert(ast_node_type_properties_table[node->type].valid_child[child->type]);
            print_ast(child);
        }
        dedent();
        print_indent();
        printf("]\n");
    }

    dedent();
}

void print_error(file_error* error)
{
    if (error->loc.filename != NULL)
    {
        char filename[256];
        basename_from_path(error->loc.filename, filename, sizeof(filename));
        printf("Error: [%s] (l: %d, c: %d): %s\n", filename, error->loc.line, error->loc.column, error->message);
    }
    else
        printf("Error: %s\n", error->message);
}

ast_node process_file(char const* filename)
{
    char buffer[1024];

    opaque_vector errors = opaque_vector_create(sizeof(file_error), file_error_destroy);
    opaque_vector tokens = lex(filename, buffer, sizeof(buffer), &errors);

    if (opaque_vector_size(&errors) > 0)
        for (file_error* error = errors.begin; error != errors.end; ++error)
            print_error(error);

    opaque_vector_clear(&errors);

    ast_node root = parse_file(filename, tokens, &errors);

    if (opaque_vector_size(&errors) > 0)
        for (file_error* error = errors.begin; error != errors.end; ++error)
            print_error(error);

    opaque_vector_destroy(&errors);
    opaque_vector_destroy(&tokens);

    return root;
}

#include "../lib/passes.h"

void process_files(char** filenames, int num_files)
{
    ast_node multifile = ast_node_create(MULTIFILE_NODE);
    for (int i = 0; i < num_files; ++i)
    {
        ast_node file = process_file(filenames[i]);
        if (file.type != EMPTY_NODE)
            ast_node_add_child(&multifile, file);
    }

    opaque_vector errors = opaque_vector_create(sizeof(file_error), file_error_destroy);

    pass_1(&multifile, &errors);

    if (opaque_vector_size(&errors) > 0)
        for (file_error* error = errors.begin; error != errors.end; ++error)
            print_error(error);

    opaque_vector_clear(&errors);

    //printf("Pass 1:\n");
    //if (opaque_vector_size(&multifile.children) > 0)
        //print_ast(&multifile);

    ast_node mf_decl_attrs, mf_decl_vars;
    pass_2(&multifile, &mf_decl_attrs, &mf_decl_vars, &errors);
    pass_3(&mf_decl_attrs, &mf_decl_vars, &errors);
    pass_4(&mf_decl_attrs, &mf_decl_vars, &errors);
    opaque_vector matchers = create_regex_matchers(&mf_decl_attrs, &errors);

    if (opaque_vector_size(&errors) > 0)
        for (file_error* error = errors.begin; error != errors.end; ++error)
            print_error(error);
        
    printf("Pass 2, decl attrs:\n");
    if (opaque_vector_size(&mf_decl_attrs.children) > 0)
        print_ast(&mf_decl_attrs);
    
    printf("Pass 2, decl vars:\n");
    if (opaque_vector_size(&mf_decl_vars.children) > 0)
        print_ast(&mf_decl_vars);
    
    printf("Pass 2, main ast:\n");
    if (opaque_vector_size(&multifile.children) > 0)
        print_ast(&multifile);

    opaque_vector_destroy(&errors);
    ast_node_destroy(&multifile);
    ast_node_destroy(&mf_decl_attrs);
    ast_node_destroy(&mf_decl_vars);
    opaque_vector_destroy(&matchers);
}

int compare_strings(const void* a, const void* b)
{
    return strcmp(*(char**)a, *(char**)b);
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
    qsort(argv, argc, sizeof(char*), compare_strings);

    process_files(argv, argc);
}

#include "../lib/errors.h"
#include "../lib/lexer.h"
#include "../lib/tokens.h"
#include "../lib/opaque_vector.h"
#include "../lib/io.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void process_file(char const* filename)
{
    opaque_vector errors = opaque_vector_create(sizeof(file_error), file_error_destroy);

    // for testing, a small buffer will reveal errors in the buffer management
    char buffer[256];
    opaque_vector tokens = lex(filename, buffer, sizeof(buffer), &errors);

    if (opaque_vector_size(&errors) > 0)
    {
        for (file_error* error = errors.begin; error != errors.end; ++error)
            print_error(error);
    }

    for (token_record const* rec = tokens.begin; rec != tokens.end; ++rec)
    {
        printf("token: %-20s", token_name(rec->token));
        printf(" line: %-10d", rec->loc.line);
        printf(" column: %-10d", rec->loc.column);

        printf(" lexeme:");
        if (rec->lexeme != NULL)
            printf(" %-20s", rec->lexeme);
        else
            printf(" %-20s", "<null>");
        printf("\n");
    }

    opaque_vector_destroy(&errors);
    opaque_vector_destroy(&tokens);
}

int compare_strings(const void* a, const void* b)
{
    return strcmp(*(char**)a, *(char**)b);
}

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        printf("Usage: %s <file> [<file> ...]\n", argv[0]);
        return 1;
    }

    ++argv;
    --argc;

        // make the output easier to look through
    qsort(argv, argc, sizeof(char*), compare_strings);

    for (int i = 0; i < argc; ++i)
    {
        printf("----- Processing file %s -----\n", argv[i]);
        process_file(argv[i]);
    }

    return 0;
}

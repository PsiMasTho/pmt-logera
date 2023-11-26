/*
    This program is used to test the lexer. It takes a file as input and prints
    the tokens to the console.
*/

#include "../src/lexer/log_lexer.h"
#include "../src/utility/utility.h" // erase_and_replace

#include "token_names.h"

#include <fmt/format.h>

#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <file>\n";
        return 1;
    }

    if(!filesystem::exists(argv[1]))
    {
        cerr << "File does not exist: " << argv[1] << '\n';
        return 1;
    }

    auto l = log_lexer(argv[1]);

    if(l.lex() != 0)
    {
        cerr << "Error lexing file: " << argv[1] << '\n';
        return 1;
    }

    auto const result = l.release_result();
    lexed_file_walker walker(result);

    // print total nr of tokens
    cout << "Total nr of tokens: " << result.get_token_count() << '\n';

    int idx = -1;
    while(1)
    {
        walker.advance();
        ++idx;
        auto const token = walker.get_cur_token_type();
        string_view const match = walker.get_cur_match();
        string text{match.data(), match.size()};

        if(token == 0)
            break;

        if(text.size() == 1)
            if(text[0] == '\0')
                text = "*NULL*";

        erase_and_replace(&text, "\n", "*newline*");

        auto tok_name = get_name(static_cast<log_tokens::Tokens_>(token));
        auto ln_nr = walker.get_cur_line_nr();

        // aligned print
        cout << fmt::format(
            "Idx: {:<20} Line_nr: {:<20} Token_int: {:<20} Token_name: {:<20} Match: {:<20}\n", idx, ln_nr, token, tok_name, text);
    }
}
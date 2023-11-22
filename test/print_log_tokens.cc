/*
    This program is used to test the lexer. It takes a file as input and prints
    the tokens to the console.
*/

#include "../src/log_lexer.h"
#include "../src/log_tokens.h"

#include <iostream>
#include <iomanip>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <file>\n";
        return 1;
    }

    if (!filesystem::exists(argv[1]))
    {
        cerr << "File does not exist: " << argv[1] << '\n';
        return 1;
    }

    auto l = log_lexer(argv[1]);

    while (int tok = l.lex())   // get all tokens
    {
        auto text = l.matched();
        cout << "Token: " << left << setw(20) << log_tokens::get_name(tok) << left << " Text: " << text << '\n';
    }

}
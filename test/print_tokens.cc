
/*
    This program is used to test the lexer. It takes a file as input and prints
    the tokens to the console.
*/

#include "../src/lexer.h"
#include "../src/tokens.h"
#include "../src/utility.h" // erase_and_replace

#include <iostream>
#include <iomanip>
#include <filesystem>

using namespace std;

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

    auto l = lexer(argv[1]);

    while (int tok = l.lex())   // get all tokens
    {
        string text = l.matched();
        erase_and_replace(&text, "\n", "*newline*");
        cout << "Token: " << left << setw(20) << tokens::get_name(tok) << left << " Text: " << text << "\n";
    }

}
#include "debug.h"
#include "main.ih"

void scanFile(std::filesystem::path const& pth)
{
	ifstream stream(pth);
    Scanner scanner(stream);

    while (int tok = scanner.lex())
    {
        switch (tok)
        {
            case Tokens::Tokens_::ATTR:
                cout << "ATTR\n";
            break;
            case Tokens::Tokens_::DATE:
                cout << "DATE\n";
            break;
            case Tokens::Tokens_::DECL_ATTR:
                cout << "DECL_ATTR\n";
            break;
            case Tokens::Tokens_::DECL_VAR:
                cout << "DECL_VAR\n";
            break;
            case Tokens::Tokens_::IDENT:
                cout << "IDENT\n";
            break;
            case Tokens::Tokens_::VALUE:
                cout << "VALUE\n";
            break;
            case Tokens::Tokens_::REGEX:
                cout << "REGEX\n";
            break;
            case '\n':
                cout << "NEWLINE\n";
            break;
            default:
                cout << scanner.matched() << '\n';
            break;
        }
    }
}
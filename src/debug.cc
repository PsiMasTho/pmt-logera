#include "debug.h"
#include "main.ih"

#include "header_scanner/header_scanner.h"
#include "log_scanner/log_scanner.h"

void scanHeader(std::filesystem::path const& pth)
{
	ifstream stream(pth);
    HeaderScanner scanner(stream);

    while (int tok = scanner.lex())
    {
        switch (tok)
        {
            case HeaderTokens::Tokens_::ATTR:
                cout << "ATTR\n";
            break;
            case HeaderTokens::Tokens_::DECL_ATTR:
                cout << "DECL_ATTR\n";
            break;
            case HeaderTokens::Tokens_::DECL_VAR:
                cout << "DECL_VAR\n";
            break;
            case HeaderTokens::Tokens_::IDENT:
                cout << "IDENT\n";
            break;
            case HeaderTokens::Tokens_::REGEX:
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

void scanLog(std::filesystem::path const& pth)
{
	ifstream stream(pth);
    LogScanner scanner(stream);

    while (int tok = scanner.lex())
    {
        switch (tok)
        {
            case LogTokens::Tokens_::ATTR:
                cout << "ATTR\n";
            break;
            case LogTokens::Tokens_::IDENT:
                cout << "IDENT\n";
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
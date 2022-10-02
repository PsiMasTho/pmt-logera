#include "main.ih"

#include "scanner/scanner.h"

enum
{
    SUCCESS = 0,
    FAIL
};

int run()
{
    Options const& opt = Options::instance();
        // set the filter, or nullptr if there is none
    std::function<bool(std::string)> filter;

    switch (opt.filterType())
    {
        case FilterType::INCLUSIVE:
            filter = [&opt](std::string str){return regex_match(str, opt.filterRegex());};
        break;
        case FilterType::EXCLUSIVE:
            filter = [&opt](std::string str){return !regex_match(str, opt.filterRegex());};
        break;
        default:
            filter = [](std::string){return true;};
        break;
    }

    ifstream headerStream(opt.headerFile());

/*
//
    ifstream logStream(opt.logFiles()[0]);
    Scanner scanner(headerStream);
    while (int tok = scanner.lex())
    {
        switch (tok)
        {
            case Scanner::Token::DECL_ATTR:
            cout << "DECL_ATTR" << '\n';
            break;
            case Scanner::Token:: DECL_VAR:
            cout << "DECL_VAR" << '\n';
            break;
            case Scanner::Token:: IDENT:
            cout << "IDENT" << '\n';
            break;
            case Scanner::Token:: VALUE:
            cout << "VALUE" << '\n';
            break;
            case Scanner::Token:: ATTR:
            cout << "ATTR" << '\n';
            break;
            case Scanner::Token:: DATE:
            cout << "DATE" << '\n';
            break;
            default:
            cout << scanner.matched() << '\n';
            break;
        };
    }
//
*/

    std::unique_ptr<HeaderData> headerData = HeaderParser(headerStream).genHeader();

    headerData->debugPrint();

    return SUCCESS;
}
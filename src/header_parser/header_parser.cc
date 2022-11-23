#include "header_parser.ih"

#include "../header_data/header_data.h"
#include <utility>

HeaderParser::HeaderParser(filesystem::path path)
    : d_scanner(path)
    , d_matched(d_scanner.matched())
    , d_ret { nullptr }
{
}

unique_ptr<HeaderData> HeaderParser::gen()
{
    d_ret.reset(new HeaderData);

    if (parse() == 0) // no error encountered
        return move(exchange(d_ret, nullptr));
    else
        return nullptr;
}

void HeaderParser::error()
{
    cerr << "Syntax error in header file: " << d_scanner.filename() << '\n';

    string matchTxt = d_matched;

    eraseAndReplace(&matchTxt, "\n", "*newline*");

    cerr << "Unexpected input: (" << matchTxt << ") encountered.\n";
    cerr << "At line: " << d_scanner.lineNr() << '\n';
}

void HeaderParser::exceptionHandler(exception const& exc)
{
    cerr << "Error in header file: " << d_scanner.filename() << '\n';
    cerr << '\t' << exc.what() << '\n';
    cerr << "At line: " << d_scanner.lineNr() << '\n';
    ABORT();
}
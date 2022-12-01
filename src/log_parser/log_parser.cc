#include "log_parser.ih"

#include "../log_data/log_data.h"
#include "../header_data/header_data.h"
#include "../date/date.h"

LogParser::LogParser(filesystem::path const& path, HeaderData const& headerData)
    : d_scanner(path)
    , d_matched { d_scanner.matched() }
    , d_logDataModifier(nullptr, headerData)
    , d_ret { nullptr }
{
}

unique_ptr<LogData> LogParser::gen()
{
    d_ret.reset(new LogData);
    d_logDataModifier.setTarget(d_ret.get());

    if (parse() == 0) // no error encountered
        return exchange(d_ret, nullptr);
    else
        return nullptr;
}

void LogParser::error()
{
    cerr << "Syntax error in log file: " << d_scanner.filename() << '\n';

    string matchTxt = d_matched;

    eraseAndReplace(&matchTxt, "\n", "*newline*");

    cerr << "Unexpected input: \"" << matchTxt << "\" encountered.\n";
    cerr << "At line: " << d_scanner.lineNr() << '\n';
}

void LogParser::exceptionHandler(exception const& exc)
{
    cerr << "Error in log file: " << d_scanner.filename() << '\n';
    cerr << '\t' << exc.what() << '\n';
    cerr << "Parsing stopped at line: " << d_scanner.lineNr() << '\n';
    ABORT();
}
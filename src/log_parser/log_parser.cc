#include "log_parser.ih"

LogParser::LogParser(std::istream& in)
:
    d_scanner(in)
{
    setDebug(false);   
}
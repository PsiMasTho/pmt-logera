#include "header_parser.ih"

HeaderParser::HeaderParser(filesystem::path path)
:
    d_stream(path),
    d_scanner(d_stream),
    d_ret{nullptr}
{}

unique_ptr<HeaderData> HeaderParser::gen()
{
    d_ret.reset(new HeaderData);
    parse();
    return move(exchange(d_ret, nullptr));
}
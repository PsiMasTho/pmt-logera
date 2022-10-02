#include "header_parser.ih"

HeaderParser::HeaderParser(std::istream& headerStream)
:
	d_scanner(headerStream),
	d_ret(new HeaderData)
{}

unique_ptr<HeaderData> HeaderParser::genHeader()
{
	parse();
	return move(exchange(d_ret, nullptr));
}
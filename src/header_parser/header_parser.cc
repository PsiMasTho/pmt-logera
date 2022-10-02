#include "header_parser.ih"

HeaderParser::HeaderParser(istream& headerStream)
:
	d_scanner(headerStream),
	d_ret{nullptr}
{}

unique_ptr<HeaderData> HeaderParser::genHeader()
{
	if (d_ret)
		throw "Error reading header";

	d_ret.reset(new HeaderData);
	parse();
	return move(exchange(d_ret, nullptr));
}
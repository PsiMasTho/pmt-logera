#include "header_parser.ih"

HeaderParser::HeaderParser(HeaderData& headerData, std::istream& headerStream)
:
	d_scanner(headerStream),
	d_headerData(headerData)
{}
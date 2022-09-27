#ifndef INCLUDED_CSV_LINE_H
#define INCLUDED_CSV_LINE_H

#include "../attribute/attribute.h"

#include <array>
#include <string>

struct LogLine
{
	std::string date;
	std::string ident;
	std::array<std::string, Attribute::_COUNT> attributes;
};

#endif
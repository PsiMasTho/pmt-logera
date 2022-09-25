#ifndef INCLUDED_CSV_DATA_H
#define INCLUDED_CSV_DATA_H

#include "../regex_filters/regex_filter_base.h"

#include <iosfwd>

class CsvData
{
public:
	void write(std::ostream& out, RegexFilterBase* filter);
};

#endif
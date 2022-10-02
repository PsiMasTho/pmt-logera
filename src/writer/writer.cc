#include "writer.h"
#include "../log_data/log_line.h"

using namespace std;

Writer::Writer(filesystem::path const& outfile, function<bool(string const&)> identFilter)
:
	d_out(outfile),
	d_identFilter(identFilter)
{}

void Writer::write(Date const& date, LogLine const& logLine)
{
		// only write if the filter accepts
	if (!d_identFilter(logLine.varName))
		return;

	d_out << date.to_string() << ";" << logLine.varName;
	for (auto const& val : logLine.attrVals)
		d_out << ";" << val;
	d_out << '\n';
}

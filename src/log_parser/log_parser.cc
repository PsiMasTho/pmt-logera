#include "log_parser.ih"

LogParser::LogParser(istream& logStream, LogData& logData)
:
	d_scanner(logStream),
	d_logData(logData)
{}
#ifndef INCLUDED_LOG_DATA_H
#define INCLUDED_LOG_DATA_H

#include "log_line.h"
#include "../date/date.h"

class HeaderData;

#include <optional>
#include <map>
#include <vector>
#include <string>
#include <iterator>
#include <utility>

class LogData
{
	using DateToLogLineMap = std::map<Date, std::vector<LogLine>>;

	HeaderData& 				   d_headerData;
	DateToLogLineMap 			   d_dateToLogLineMap;
	std::optional<Date> 		   d_lastDate;
	std::string					   d_lastVar;

public:
	#include "const_iterator.hi"

	LogData(HeaderData& headerData);

	const_iterator cbegin();
	const_iterator cend();

	void setActiveDate(Date const& date);
	void startNewLogLineForNewVar(std::string const& varName);
	void startNewLogLineForActiveVar();
	void addAttrToActiveVar(std::string const& attrName, std::string const& attrVal);

	void debugPrint() const;
};

#endif
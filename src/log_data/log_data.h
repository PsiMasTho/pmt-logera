#ifndef INCLUDED_LOG_DATA_H
#define INCLUDED_LOG_DATA_H

#include "../date/date.h"

class HeaderData;

#include <optional>
#include <map>
#include <vector>
#include <string>

class LogData
{
	struct LogLine
	{
		std::string 			 varName;
		std::vector<std::string> attrVals;
	};

	using DateToLogLineMap = std::map<Date, std::vector<LogLine>>;

	HeaderData& 				   d_headerData;
	DateToLogLineMap 			   d_dateToLogLineMap;
	std::optional<Date> 		   d_lastDate;
	std::string					   d_lastVar;

public:
	LogData(HeaderData& headerData);

	void setActiveDate(Date const& date);
	void startNewLogLineForNewVar(std::string const& varName);
	void startNewLogLineForActiveVar();
	void addAttrToActiveVar(std::string const& attrName, std::string const& attrVal);

	void debugPrint() const;
};

#endif
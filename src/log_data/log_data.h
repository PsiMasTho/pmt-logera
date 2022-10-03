#ifndef INCLUDED_LOG_DATA_H
#define INCLUDED_LOG_DATA_H

#include "../date/date.h"

class HeaderData;

#include <optional>
#include <vector>
#include <string>
#include <utility>
#include <span>

class LogData
{
public:
        //                   <name, attribute values>
    using LogLine = std::pair<std::string, std::vector<std::string>>;
private:
    std::vector<std::pair<Date, LogLine>> d_logLines;
    HeaderData&                           d_headerData;

    std::optional<Date>                   d_curDate;
    std::optional<std::string>            d_curVar;

public:
    LogData(HeaderData& headerData);

    void setActiveDate(Date const& date);
    void startNewLogLineForNewVar(std::string const& varName);
    void startNewLogLineForActiveVar();
    void addAttrToActiveVar(std::string const& attrName, std::string const& attrVal);

    std::vector<std::pair<Date, LogLine>> const& getLines() const;

    void sortLinesByDate();
};

#endif

#include "log_data.h"

#include "../header_data/header_data.h"

#include <algorithm>
#include <execution>

using namespace std;

LogData::LogData(HeaderData& headerData)
:
    d_logLines{},
    d_headerData{headerData},
    d_curDate{nullopt},
    d_curVar{nullopt}
{}

void LogData::setActiveDate(Date const& date)
{
    d_curDate = date;
}

void LogData::startNewLogLineForNewVar(std::string const& varName)
{
    if (!d_curDate.has_value())
        throw "Log with no date detected";

    d_curVar = varName;
    d_logLines.push_back(pair(d_curDate.value(), pair(d_curVar.value(), vector<string>(d_headerData.getAttributes().getCount(), ""s))));
}

void LogData::startNewLogLineForActiveVar()
{
    d_logLines.push_back(pair(d_curDate.value(), pair(d_curVar.value(), vector<string>(d_headerData.getAttributes().getCount(), ""s))));
}

void LogData::addAttrToActiveVar(std::string const& attrName, std::string const& attrVal)
{
    if (!d_curVar.has_value())
        throw "Attempting to add attribute without a variable";

    if (!d_headerData.doesVarHaveAttr(d_curVar.value(), attrName))
        throw "Variable: "s + d_curVar.value() + " does not have attribute: "s + attrName;

    size_t const idx = d_headerData.getAttributes().getIdx(attrName);
    if (!d_headerData.getAttributes().validValue(idx, attrVal))
        throw "Invalid value: "s + attrVal + " for Attribute: " + attrName;

    d_logLines.back().second.second[idx] = attrVal;
}

vector<pair<Date, LogData::LogLine>> const& LogData::getLines() const
{
    return d_logLines;
}

void LogData::sortLinesByDate()
{
    stable_sort(execution::par_unseq, begin(d_logLines), end(d_logLines), [](auto const& lhs, auto const& rhs){return lhs.first < rhs.first;});
}
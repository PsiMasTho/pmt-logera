#include "log_data_modifier.h"
#include "../log_data/log_data.h"
#include "../header_data/header_data.h"
#include "../date/date.h"

#include <string>

using namespace std;

LogDataModifier::LogDataModifier(LogData* target, HeaderData const& headerData)
:
    d_target{target},
	d_headerData{headerData}
{}

void LogDataModifier::setTarget(LogData* target)
{
    d_target = target;
}

void LogDataModifier::setDate(Date const& date)
{
    d_target->d_date = date;
}

void LogDataModifier::startNewLineForNewVar(std::string const& varName)
{
    if (!d_target->d_date.ok())
        throw "Log with bad date detected"s;

        // make enough room for all attributes + var name
    d_target->d_lines.emplace_back(d_headerData.getAttributes().getCount() + 1, ""s);
    d_target->d_lines.back().front().assign(varName);
}

void LogDataModifier::startNewLineForOldVar()
{
    if (!d_target->d_date.ok())
        throw "Log with bad date detected"s;
    if (d_target->d_lines.empty())
        throw "Adding new line without old variable"s;

    string const& curVar = d_target->d_lines.back().front();
    
    d_target->d_lines.emplace_back(d_headerData.getAttributes().getCount() + 1, ""s);
    d_target->d_lines.back().front().assign(curVar);
}

void LogDataModifier::addAttrToCurrentLine(std::string const& attrName, std::string const& attrVal)
{
    if (!d_target->d_date.ok())
        throw "Log with bad date detected"s;
    if (d_target->d_lines.back().front().empty())
        throw "Attempting to add attribute with no variable"s;

    string const& curVar = d_target->d_lines.back().front();

    if (!d_headerData.doesVarHaveAttr(curVar, attrName))
        throw "Variable: \""s + curVar + "\" does not have attribute: \""s + attrName + '\"';

    size_t const idx = d_headerData.getAttributes().getIdx(attrName);
    if (!d_headerData.getAttributes().validValue(idx, attrVal))
        throw "Invalid value: \""s + attrVal + "\". For attribute: \"" + attrName + '\"';

    d_target->d_lines.back()[idx + 1] = attrVal;
}
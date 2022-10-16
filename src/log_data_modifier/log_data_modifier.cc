#include "log_data_modifier.h"
#include "../date/date.h"
#include "../header_data/header_data.h"
#include "../log_data/log_data.h"

#include <exception>
#include <string>

using namespace std;

LogDataModifier::LogDataModifier(LogData* target, HeaderData const& headerData)
    : d_target{target}
    , d_headerData{headerData}
    , d_activeVar{}
{ }

void LogDataModifier::setTarget(LogData* target)
{
    d_target = target;
}

void LogDataModifier::setDate(Date const& date)
{
    d_target->d_date = date;
}

void LogDataModifier::setActiveVar(string const& varName)
{
    if(!d_target->d_date.ok())
        throw runtime_error("Log with bad date detected");

    d_activeVar = varName;
}

void LogDataModifier::addAttrToNewLine(string const& attrName, string const& attrVal)
{
    if(!d_target->d_date.ok())
        throw runtime_error("Log with bad date detected");
    if(d_activeVar.empty())
        throw runtime_error("Adding new line without active variable");

    d_target->d_lines.emplace_back(d_headerData.getAttributes().getCount() + 1);
    d_target->d_lines.back().set(0, d_activeVar);

    addAttrToCurrentLine(attrName, attrVal);
}

void LogDataModifier::addAttrToCurrentLine(string const& attrName, string const& attrVal)
{
    if(!d_target->d_date.ok())
        throw runtime_error("Log with bad date detected"s);

    if(!d_target->d_lines.back().exists(0))
        throw runtime_error("Attempting to add attribute with no variable"s);

    if(!d_headerData.doesVarHaveAttr(d_activeVar, attrName))
        throw runtime_error("Variable: \""s + d_activeVar + "\" does not have attribute: \""s +
                            attrName + '\"');

    size_t const idx = d_headerData.getAttributes().getIdx(attrName);
    if(!d_headerData.getAttributes().validValue(idx, attrVal))
        throw runtime_error("Invalid value: \""s + attrVal + "\". For attribute: \"" + attrName +
                            '\"');

    d_target->d_lines.back().set(idx + 1, attrVal);
}
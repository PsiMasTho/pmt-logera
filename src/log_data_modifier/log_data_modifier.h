#ifndef INCLUDED_LOG_DATA_MODIFIER
#define INCLUDED_LOG_DATA_MODIFIER

#include <string>

class HeaderData;
class LogData;
class Date;

class LogDataModifier
{
    LogData* d_target;
    HeaderData const& d_headerData;

    std::string d_activeVar;

public:
    LogDataModifier(LogData* target, HeaderData const& headerData);

    void setTarget(LogData* target);

    void setDate(Date const& date);

    void setActiveVar(std::string const& varName);
    void addAttrToNewLine(std::string const& attrName, std::string const& attrVal);
    void addAttrToCurrentLine(std::string const& attrName, std::string const& attrVal);
};

#endif
#ifndef INCLUDED_LOG_DATA_MODIFIER
#define INCLUDED_LOG_DATA_MODIFIER

#include <iosfwd> // std::string

class HeaderData;
class LogData;
class Date;

class LogDataModifier
{
    LogData*          d_target;
    HeaderData const& d_headerData;

public:
    LogDataModifier(LogData* target, HeaderData const& headerData);

    void setTarget(LogData* target);

    void setDate(Date const& date);
    void startNewLineForNewVar(std::string const& varName);
    void startNewLineForOldVar();
    void addAttrToCurrentLine(std::string const& attrName, std::string const& attrVal);
};

#endif
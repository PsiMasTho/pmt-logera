#ifndef INCLUDED_LOG_LINE_H
#define INCLUDED_LOG_LINE_H

#include <string>
#include <vector>

struct LogLine
{
    std::string              varName;
    std::vector<std::string> attrVals;
};

#endif
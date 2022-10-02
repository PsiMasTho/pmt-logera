#include "log_data.h"

#include "../header_data/header_data.h"

#include <iostream>

using namespace std;

LogData::LogData(HeaderData& headerData)
:
    d_headerData{headerData},
    d_dateToLogLineMap{},
    d_lastDate{nullopt},
    d_lastVar{}
{}

LogData::const_iterator LogData::cbegin()
{
    return const_iterator(this, d_dateToLogLineMap.cbegin());
}

LogData::const_iterator LogData::cend()
{
    return const_iterator(this, d_dateToLogLineMap.cend());
}


void LogData::setActiveDate(Date const& date)
{
    d_dateToLogLineMap.insert({date, {}});
    d_lastDate.emplace(date);
}

void LogData::startNewLogLineForNewVar(std::string const& varName)
{
    d_dateToLogLineMap[d_lastDate.value()].push_back({varName, vector<string>(d_headerData.getAttrCount(), ""s)});
    d_lastVar = varName;
}

void LogData::startNewLogLineForActiveVar()
{
    d_dateToLogLineMap[d_lastDate.value()].push_back({d_lastVar, vector<string>(d_headerData.getAttrCount(), ""s)});
}

void LogData::addAttrToActiveVar(std::string const& attrName, std::string const& attrVal)
{
    if (size_t idx; d_headerData.doesAttrExist(&idx, attrName))
        if (d_headerData.doesVarHaveAttr(d_lastVar, attrName))
            d_dateToLogLineMap[d_lastDate.value()].back().attrVals[idx] = attrVal;
}

void LogData::debugPrint() const
{
    for (auto [date, loglines] : d_dateToLogLineMap)
    {
        cout << date.to_string() << '\n';
        for (auto line : loglines)
        {
            cout << "\tName: " << line.varName << '\n';
            for (auto attr : line.attrVals)
                cout << "\t\tAttribute val: " << attr << '\n';
        }
    }
}

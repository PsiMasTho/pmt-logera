//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_LOG_DATA_MODIFIER
#define INCLUDED_LOG_DATA_MODIFIER

#include <string>

class header_data;
class LogData;
class Date;

class LogDataModifier
{
    LogData* m_target;
    header_data const& m_headerData;

    std::string m_activeVar;

public:
    LogDataModifier(LogData* target, header_data const& headerData);

    void setTarget(LogData* target);

    void setDate(Date const& date);

    void setActiveVar(std::string const& var_name);
    void addAttrToNewLine(std::string const& attr_name, std::string const& attrVal);
    void addAttrToCurrentLine(std::string const& attr_name, std::string const& attrVal);
};

#endif
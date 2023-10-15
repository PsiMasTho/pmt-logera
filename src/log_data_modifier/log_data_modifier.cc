//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "log_data_modifier.h"
#include "../date/date.h"
#include "../header_data/header_data.h"
#include "../log_data/log_data.h"

#include <exception>
#include <string>

using namespace std;

LogDataModifier::LogDataModifier(LogData* target, header_data const& headerData)
    : m_target{target}
    , m_headerData{headerData}
    , m_activeVar{}
{ }

void LogDataModifier::setTarget(LogData* target)
{
    m_target = target;
}

void LogDataModifier::setDate(Date const& date)
{
    m_target->m_date = date;
}

void LogDataModifier::setActiveVar(string const& var_name)
{
    m_activeVar = var_name;
}

void LogDataModifier::addAttrToNewLine(string const& attr_name, string const& attrVal)
{
    if(m_activeVar.empty())
        throw runtime_error("Adding new line without active variable");

    m_target->m_lines.emplace_back(m_headerData.get_attributes().getCount() + 1);
    m_target->m_lines.back().set(0, m_activeVar);

    addAttrToCurrentLine(attr_name, attrVal);
}

void LogDataModifier::addAttrToCurrentLine(string const& attr_name, string const& attrVal)
{
    if(!m_target->m_lines.back().exists(0))
        throw runtime_error("Attempting to add attribute with no variable"s);

    if(!m_headerData.does_var_have_attr(m_activeVar, attr_name))
        throw runtime_error("Variable: \""s + m_activeVar + R"(" does not have attribute: ")"s +
                            attr_name + '\"');

    size_t const idx = m_headerData.get_attributes().getIdx(attr_name);
    if(!m_headerData.get_attributes().validValue(idx, attrVal))
        throw runtime_error("Invalid value: \""s + attrVal + "\". For attribute: \"" + attr_name +
                            '\"');

    m_target->m_lines.back().set(idx + 1, attrVal);
}
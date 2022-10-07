#include "header_data.h"

using namespace std;

#include <algorithm>

HeaderData::HeaderData()
:
    d_vars{},
    d_attrs{},
    d_lastVarItr{d_vars.end()}
{}

void HeaderData::addVar(string const& varName)
{
    auto const [itr, success] = d_vars.insert({varName, {}});
    if (!success)
        throw "Failed declaring variable: "s + varName;
    else
        d_lastVarItr = itr;
}

void HeaderData::addAttr(string const& attrName)
{
    d_attrs.addAttr(attrName);
}

void HeaderData::addRegexToLastAttr(std::string const& expr)
{
    d_attrs.addRegexToLastAttr(expr);
}

void HeaderData::addAttrToLastVar(std::string const& attrName)
{
    if (d_lastVarItr == d_vars.end())
        throw "Attemtpting to add attribute without a variable: "s + attrName;

    d_lastVarItr->second[d_attrs.getIdx(attrName)] = true;
}

bool HeaderData::doesVarHaveAttr(std::string const& varName, std::string const& attrName) const
{
    auto const itr = d_vars.find(varName);
    if (itr == d_vars.end())
        throw "Unknown variable queried: "s + varName;
    else
        return itr->second[d_attrs.getIdx(attrName)];
}

Attributes const& HeaderData::getAttributes() const
{
    return d_attrs;
}

#include <ostream>
void HeaderData::debugPrint(ostream& out) const
{
    for (auto [varName, idxBits] : d_vars)
    {
        out << "VAR: " << varName << '\n';
        out << "\tATTRS: " << idxBits.to_string() << '\n';
    }
}
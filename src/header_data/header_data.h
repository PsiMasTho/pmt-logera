//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_HEADER_DATA_H
#define INCLUDED_HEADER_DATA_H

#include "../attributes/attributes.h"

#include <bitset>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <iosfwd>

#ifndef MAX_ATTRIBUTES
#    define MAX_ATTRIBUTES 128
#endif

class HeaderData
{
    using VarToAttrIdxMap = std::unordered_map<std::string, std::bitset<MAX_ATTRIBUTES>>;

    // <var name, list of associated attribute indices from d_attrs>
    VarToAttrIdxMap d_vars;
    Attributes d_attrs;
    VarToAttrIdxMap::iterator d_lastVarItr;

public:
    HeaderData();
    void addVar(std::string const& varName);
    void addAttr(std::string const& attrName);
    void addRegexToLastAttr(std::string const& expr);
    void addAttrToLastVar(std::string const& attrName);

    bool doesVarHaveAttr(std::string const& varName, std::string const& attrName) const;

    Attributes const& getAttributes() const;

    void debugPrint(std::ostream& out) const;
};

#endif
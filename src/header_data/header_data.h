#ifndef INCLUDED_HEADER_DATA_H
#define INCLUDED_HEADER_DATA_H

#include "../attribute/attribute.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

class HeaderData
{
    using VarToIdxVecMap = std::unordered_map<std::string, std::unordered_set<size_t>>;

    std::vector<Attribute>   d_attrs;

        // <var name, list of associated attribute indices from d_attrs>
    VarToIdxVecMap           d_vars;
    VarToIdxVecMap::iterator d_lastVarItr;
    

public:
    HeaderData();
    void addVar(std::string const& varName);

    void addAttr(std::string const& attrName);
    void addAttr(std::string const& attrName, std::regex attrRegex);

    void addAttrToLastVar(std::string const& attrName);

    bool doesVarHaveAttr(std::string const& varName, std::string const& attrName);

    void debugPrint() const;

    bool doesAttrExist(size_t* idx, std::string const& attrName) const;

    size_t getAttrCount() const;
    std::string const& getAttrName(size_t idx) const;
};

#endif
#include "header_data.h"

using namespace std;

#include <algorithm>

HeaderData::HeaderData()
:
	d_attrs{},
	d_vars{},
	d_lastVarItr{d_vars.end()}
{}

void HeaderData::addVar(string const& varName)
{
	auto const [itr, success] = d_vars.insert({varName, {}});
	if (!success)
		throw "Bad header file";
	else
		d_lastVarItr = itr;
}

void HeaderData::addAttr(string const& attrName)
{
	d_attrs.emplace_back(attrName);
}

void HeaderData::addAttr(std::string const& attrName, std::regex attrRegex)
{
	d_attrs.emplace_back(attrName, attrRegex);
}

void HeaderData::addAttrToLastVar(std::string const& attrName)
{
	if (d_lastVarItr != d_vars.end())
		if (size_t idx; doesAttrExist(&idx, attrName))
			d_lastVarItr->second.insert(idx);
}

bool HeaderData::doesVarHaveAttr(std::string const& varName, std::string const& attrName)
{
	auto const itr = d_vars.find(varName);
	if (itr == d_vars.end())
		throw "Unknown variable queried";
	else
	{
		if (size_t idx; doesAttrExist(&idx, attrName))
			return itr->second.contains(idx);
		else
			throw "Unknow attribute queried";
	}
}

#include <iostream>
void HeaderData::debugPrint() const
{
	for (auto [varName, idxSet] : d_vars)
	{
		cout << "VAR: " << varName << '\n';
		for (auto idx : idxSet)
			cout << '\t' << d_attrs[idx].name() << '\n';
	}
}

bool HeaderData::doesAttrExist(size_t* idx, std::string const& attrName) const
{
	auto const itr = find_if(begin(d_attrs), end(d_attrs), [&attrName](auto const& elem){return elem.name() == attrName;});

	if (itr == d_attrs.end())
		return false;
	else
	{
		if (idx)
			*idx = distance(d_attrs.begin(), itr);
		return true;
	}
}

size_t HeaderData::getAttrCount() const
{
	return d_attrs.size();
}
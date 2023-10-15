//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "attributes.h"

#include <algorithm>
#include <exception>
#include <regex>

using namespace std;

attributes::attributes()
    : m_attrs{}
{ }

void attributes::add_attr(string const& name)
{
    m_attrs.push_back(pair(name, vector<RegexMatchChecker>{}));
}

void attributes::add_regex_to_last_attr(std::string const& expr)
{
    if(m_attrs.empty())
        throw runtime_error("Trying to add regex without an attribute: "s + expr);

    m_attrs.back().second.emplace_back([rexpr = regex(expr, regex::optimize)](string const& expr) {
        return regex_match(expr, rexpr);
    });
}

bool attributes::validValue(size_t idx, string const& value) const
{
    // if no regex is defined, then assume it's a match
    if(m_attrs[idx].second.empty())
        return true;

    // does any regex match?
    return any_of(begin(m_attrs[idx].second), end(m_attrs[idx].second), [value](auto const& fun) {
        return fun(value);
    });
}

size_t attributes::getCount() const
{
    // assuming m_names.size() == m_checkValueRegexFuncs.size()
    return m_attrs.size();
}

size_t attributes::getIdx(std::string const& name) const
{
    auto const itr = find_if(
        begin(m_attrs), end(m_attrs), [&name](auto const& attr) { return attr.first == name; });

    if(itr == end(m_attrs))
        throw runtime_error("Queried unknown attribute: "s + name);

    // itr >= m_attrs.begin()
    // so the distance will not be negative
    return static_cast<size_t>(distance(begin(m_attrs), itr));
}

string const& attributes::getName(size_t idx) const
{
    return m_attrs[idx].first;
}

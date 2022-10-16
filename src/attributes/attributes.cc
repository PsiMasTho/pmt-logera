#include "attributes.h"

#include <algorithm>
#include <execution>
#include <exception>
#include <regex>

using namespace std;

namespace {
class RegexMatchLambda {
    regex const d_regex;

public:
    RegexMatchLambda(string const& rexpr);
    bool operator()(string const& str) const;
};
}

Attributes::Attributes()
    : d_attrs {}
{
}

void Attributes::addAttr(string const& name)
{
    d_attrs.push_back(pair(name, vector<RegexMatchChecker> {}));
}

void Attributes::addRegexToLastAttr(std::string const& expr)
{
    if (d_attrs.empty())
        throw runtime_error(
            "Trying to add regex without an attribute: "s + expr);

    d_attrs.back().second.push_back(RegexMatchLambda(expr));
}

bool Attributes::validValue(size_t idx, string const& value) const
{
    // if no regex is defined, then assume it's a match
    if (d_attrs[idx].second.empty()) return true;

    // does any regex match?
    return any_of(begin(d_attrs[idx].second), end(d_attrs[idx].second),
        [value](auto const& fun) { return fun(value); });
}

size_t Attributes::getCount() const
{
    // assuming d_names.size() == d_checkValueRegexFuncs.size()
    return d_attrs.size();
}

size_t Attributes::getIdx(std::string const& name) const
{
    auto const itr = find_if(begin(d_attrs), end(d_attrs),
        [&name](auto const& attr) { return attr.first == name; });

    if (itr == end(d_attrs))
        throw runtime_error("Queried unknown attribute: "s + name);

    return distance(begin(d_attrs), itr);
}

string const& Attributes::getName(size_t idx) const
{
    return d_attrs[idx].first;
}

namespace {
RegexMatchLambda::RegexMatchLambda(string const& rexpr)
    : d_regex(rexpr, regex::optimize)
{
}

bool RegexMatchLambda::operator()(string const& str) const
{
    return regex_match(str, d_regex);
}
}

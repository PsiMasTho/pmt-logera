#include "attributes.h"

#include <algorithm>
#include <utility>

using namespace std;

namespace
{
    template< typename T>
    typename vector<T>::iterator
    insert_sorted(vector<T>& vec, T const& item)
    {
        return vec.insert(upper_bound(begin(vec), end(vec), item, less<>{}), item);
    }
}

void Attributes::push_back(string const& name)
{
    insertSorted(name, [](string const&){return true;});
}

void Attributes::push_back(string const& name, regex expr)
{
    insertSorted(name, [&expr](string const& str){return regex_match(str, expr);});
}

bool Attributes::validValue(size_t idx, string const& value) const
{
    return d_checkValueRegexFuncs[idx](value);
}

size_t Attributes::getCount() const
{
        // assuming d_names.size() == d_checkValueRegexFuncs.size()
    return d_names.size();
}

size_t Attributes::getIdx(std::string const& name) const
{
    auto const [begitr, enditr] = equal_range(begin(d_names), end(d_names), name, less<>{});

    if (distance(begitr, enditr) != 1)
        throw "Queried unknown attribute: "s + name;

    return distance(begin(d_names), begitr);
}

string const& Attributes::getName(size_t idx) const
{
    return d_names[idx];
}

void Attributes::insertSorted(std::string const& name, CheckStringFun fun)
{
    auto const nameItr = insert_sorted(d_names, name);
    auto const funcItr = d_checkValueRegexFuncs.begin() + distance(begin(d_names), nameItr);
    d_checkValueRegexFuncs.insert(funcItr, fun);
}

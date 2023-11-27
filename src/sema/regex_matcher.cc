#include "regex_matcher.h"

#include <algorithm>

using namespace std;

void regex_matcher::add_regex(std::string const& expr)
{
    m_regexes.emplace_back(expr, regex::optimize);
}

auto regex_matcher::operator()(std::string const& str) const -> bool
{
    // if no regex is defined, then assume it's a match
    if(m_regexes.empty())
        return true;

    // does any regex match?
    return any_of(begin(m_regexes), end(m_regexes), [&str](auto const& rexpr) { return regex_match(str, rexpr); });
}

auto construct_regex_matchers(header_data const& hd) -> std::vector<regex_matcher>
{
    vector<regex_matcher> ret;
    ret.reserve(hd.attrs.size());

    for(auto const& attr : hd.attrs)
    {
        ret.emplace_back();
        for(auto const& expr : attr.reg_exprs)
            ret.back().add_regex(expr);
    }
}
#ifndef INCLUDED_EXCLUSIVE_REGEX_FILTER_H
#define INCLUDED_EXCLUSIVE_REGEX_FILTER_H

#include "regex_filter_base.h"

class ExclusiveRegexFilter final : public RegexFilterBase
{
public:
    explicit ExclusiveRegexFilter(std::regex expr);

    bool accept(std::string const& str) override;
};

inline ExclusiveRegexFilter::ExclusiveRegexFilter(std::regex expr)
:
    RegexFilterBase(expr)
{}

inline bool ExclusiveRegexFilter::accept(std::string const& str)
{
    return !regex_match(str, d_regex);
}

#endif
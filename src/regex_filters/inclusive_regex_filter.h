#ifndef INCLUDED_INCLUSIVE_REGEX_FILTER_H
#define INCLUDED_INCLUSIVE_REGEX_FILTER_H

#include "regex_filter_base.h"

class InclusiveRegexFilter final : public RegexFilterBase
{
public:
	explicit InclusiveRegexFilter(std::regex expr);

	bool accept(std::string const& str) override;
};

inline InclusiveRegexFilter::InclusiveRegexFilter(std::regex expr)
:
	RegexFilterBase(expr)
{}

inline bool InclusiveRegexFilter::accept(std::string const& str)
{
	return regex_match(str, d_regex);
}

#endif
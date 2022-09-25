#ifndef INCLUDED_REGEX_FILTER_BASE_H
#define INCLUDED_REGEX_FILTER_BASE_H

#include <string>
#include <regex>

class RegexFilterBase
{
protected:
		// needs to be non const to allow implicitly generated cpy assignment
	std::regex d_regex;

public:
	explicit RegexFilterBase(std::regex expr);

		// return true if a given string should be accepted based on the regex
	[[nodiscard]] virtual bool accept(std::string const& str) = 0;
};

inline RegexFilterBase::RegexFilterBase(std::regex expr)
:
	d_regex(expr)
{}

#endif
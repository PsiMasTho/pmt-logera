#include "attribute.h"

using namespace std;

Attribute::Attribute(string const& name)
:
	d_name{name},
	d_checkValueRegex([](string const&){return true;})
{}

Attribute::Attribute(string const& name, regex expr)
:
	d_name{name},
	d_checkValueRegex([&expr](string const& str){return regex_match(str, expr);})
{}

string const& Attribute::name() const
{
	return d_name;
}

bool Attribute::validValue(string const& value) const
{
	return d_checkValueRegex(value);
}

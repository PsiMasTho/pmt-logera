#ifndef INCLUDED_ATTRIBUTE_H
#define INCLUDED_ATTRIBUTE_H

#include <string>
#include <optional>

namespace Attribute
{
	#include "attribute_enums.hi"

		// checks if the given string %attr is an attribute, returns
		// the ID inside the std::optional if yes
	std::optional<ID> 	 validateAttrStr(std::string const& attr);
	
	bool 				 validateValueStr(ID attr, std::string const& value);
}

#endif
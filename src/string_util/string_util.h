#ifndef INCLUDED_STRING_UTIL_H
#define INCLUDED_STRING_UTIL_H

#include <string>
#include <string_view>

void eraseAndReplace(std::string* target, std::string const& toErase, std::string const& toReplace);

// Concats rhs to lhs with delim inbetween
void appendToString(std::string& lhs, std::string_view const rhs, std::string const& delim);

#endif
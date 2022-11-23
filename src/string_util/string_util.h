#ifndef INCLUDED_STRING_UTIL_H
#define INCLUDED_STRING_UTIL_H

#include <iosfwd>

void eraseAndReplace(std::string* target, std::string const& toErase, std::string const& toReplace);

#endif
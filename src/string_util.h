#ifndef INCLUDED_STRING_UTIL_H
#define INCLUDED_STRING_UTIL_H

#include <iosfwd>

std::string& removeCh(std::string& str, char ch);
std::string& trim(std::string& str, const char* ws = " \t\n\r\f\v");
void eraseAndReplace(std::string* target, std::string const& toErase, std::string const& toReplace);

#endif
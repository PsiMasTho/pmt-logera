#ifndef INCLUDED_UTIL_H
#define INCLUDED_UTIL_H

#include <string>

std::string& removeCh(std::string& str, char ch);
std::string& trim(std::string& str, const char* ws = " \t\n\r\f\v");

#endif
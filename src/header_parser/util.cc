#include "util.h"

#include <algorithm>

using namespace std;

namespace
{
		// trim from left
	string& _ltrim(string& str, const char* ws = " \t\n\r\f\v");

		// trim from right
	string& _rtrim(string& str, const char* ws = " \t\n\r\f\v");
}

std::string& removeCh(string& str, char ch)
{
	str.erase(remove(begin(str), end(str), ch), end(str));
	return str;
}

    // trim from left & right
string& trim(string& str, const char* ws)
{
    return _ltrim(_rtrim(str, ws), ws);
}

namespace
{
	string& _ltrim(string& str, const char* ws)
	{
		str.erase(0, str.find_first_not_of(ws));
		return str;
	}

	string& _rtrim(string& str, const char* ws)
	{
		str.erase(str.find_last_not_of(ws) + 1);
		return str;
	}
}
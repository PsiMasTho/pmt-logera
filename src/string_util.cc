#include "string_util.h"

#include <algorithm>
#include <string>

using namespace std;

namespace {
// trim from left
string& _ltrim(string& str, const char* ws = " \t\n\r\f\v");

// trim from right
string& _rtrim(string& str, const char* ws = " \t\n\r\f\v");
}

string& removeCh(string& str, char ch)
{
    str.erase(remove(begin(str), end(str), ch), end(str));
    return str;
}

// trim from left & right
string& trim(string& str, const char* ws)
{
    return _ltrim(_rtrim(str, ws), ws);
}

void eraseAndReplace(
    string* target, string const& toErase, string const& toReplace)
{
    size_t const pos = target->find(toErase);
    if (pos != string::npos) {
        target->erase(pos, toErase.size());
        target->insert(pos, toReplace);
    }
}

namespace {
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
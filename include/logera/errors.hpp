#pragma once

#include <cstdarg>
#include <string>

namespace error
{

enum code
{
    IO_CANNOT_OPEN_FILE,
    IO_CANNOT_READ_FILE,

    PARSER_UNEXPECTED_TOKEN,

    SEMA_DUPLICATE_FILENAME,
    SEMA_DATE_NOT_IN_FILENAME_ORDER,
    SEMA_INVALID_DATE,
    SEMA_VALUE_LIST_WITHOUT_IDENT,
    SEMA_MULTIPLE_DATES,
    SEMA_DATE_NOT_FIRST_IN_FILE,
    SEMA_NO_DATE_BEFORE_ENTRY,
    SEMA_IDENT_WITHOUT_VALUE_LIST,
    SEMA_UNDECLARED_ATTR_IN_DECL,
    SEMA_UNDECLARED_ATTR_IN_DECL_W_HINT,
    SEMA_UNDECLARED_ATTR_IN_ENTRY,
    SEMA_UNDECLARED_ATTR_IN_ENTRY_W_HINT,
    SEMA_UNDECLARED_VAR,
    SEMA_UNDECLARED_VAR_W_HINT,
    SEMA_DUPLICATE_ATTR,
    SEMA_DUPLICATE_VAR,
    SEMA_DUPLICATE_ATTR_VALUE,
    SEMA_DUPLICATE_VAR_VALUE,
    SEMA_NO_ATTRIBUTES_DECLARED,
    SEMA_NO_VARIABLES_DECLARED,
    SEMA_REGCOMP_FAILED,

    _CODE_COUNT
};

// clang-format off
inline char const* const fmts[_CODE_COUNT] = {
    "cannot open file \'%s\'",
    "error reading file \'%s\'",

    "[%s] (l:%d, c:%d) unexpected token",

    "duplicate filename \'%s\' encountered",
    "[%s] (l:%d, c:%d) date not in filename order",
    "[%s] (l:%d, c:%d) invalid date encountered \'%s\'",
    "[%s] (l:%d, c:%d) value list encountered without identifier",
    "[%s] (l:%d, c:%d) multiple dates encountered",
    "[%s] (l:%d, c:%d) date not first in file",
    "[%s] (l:%d, c:%d) no date encountered before entry",
    "[%s] (l:%d, c:%d) identifier encountered without value list",
    "[%s] (l:%d, c:%d) undeclared attribute \'%s\' encountered in declaration for variable \'%s\'",
    "[%s] (l:%d, c:%d) undeclared attribute \'%s\' encountered in declaration for variable \'%s\', did you mean \'%s\'?",
    "[%s] (l:%d, c:%d) undeclared attribute \'%s\' encountered in entry for variable \'%s\'",
    "[%s] (l:%d, c:%d) undeclared attribute \'%s\' encountered in entry for variable \'%s\', did you mean \'%s\'?",
    "[%s] (l:%d, c:%d) undeclared variable \'%s\' encountered",
    "[%s] (l:%d, c:%d) undeclared variable \'%s\' encountered, did you mean \'%s\'?",
    "[%s] (l:%d, c:%d) duplicate attribute \'%s\' encountered",
    "[%s] (l:%d, c:%d) duplicate variable \'%s\' encountered",
    "[%s] (l:%d, c:%d) duplicate attribute value \'%s\' encountered",
    "[%s] (l:%d, c:%d) duplicate variable value \'%s\' encountered",
    "no attributes declared",
    "no variables declared",
    "[%s] (l:%d, c:%d) failed to compile regex: \'%s\', for attr: \'%s\'. %s"
};
// clang-format on

struct record
{
    std::string msg;
    int         code;

    record(int code, ...);
};

} // namespace error

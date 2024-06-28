#pragma once

namespace pmt
{

enum warning_code : int
{
  DATE_NOT_IN_FILENAME_ORDER,
  DATE_NOT_FIRST_IN_FILE,
  INVALID_DATE,
  REDEFINING_DATE,
  NO_DATE_BEFORE_ENTRY,
  NO_ENTRIES_IN_FILE,
  VARIABLE_DECLARATION_ORDER_VIOLATION,
  ATTRIBUTE_DECLARATION_ORDER_VIOLATION,
  UNDECLARED_ATTRIBUTE_IN_VARIABLE_DECLARATION,
  UNDECLARED_ATTRIBUTE_IN_VARIABLE_DECLARATION_W_HINT,
  DUPLICATE_ATTRIBUTE_NAME_IN_ENTRY,
  ENTRY_START_WITHOUT_VALUE_LIST,
  VALUE_LIST_WITHOUT_ENTRY_START,
  REGCOMP_FAILED,
  REGEX_MISMATCH,
  UNDECLARED_VARIABLE_IN_ENTRY_GLOBAL,
  UNDECLARED_VARIABLE_IN_ENTRY_GLOBAL_W_HINT,
  UNDECLARED_ATTRIBUTE_IN_ENTRY_GLOBAL,
  UNDECLARED_ATTRIBUTE_IN_ENTRY_GLOBAL_W_HINT,
  UNDECLARED_ATTRIBUTE_IN_ENTRY_W_HINT,
  UNDECLARED_ATTRIBUTE_IN_ENTRY_LOCAL_W_HINT,
  UNDECLARED_ATTRIBUTE_IN_ENTRY_LOCAL,
  DUPLICATE_FILEPATH,
  DUPLICATE_ATTRIBUTE_DECLARATION_IDENTIFIER,
  DUPLICATE_DECLARATION_IDENTIFIER,
  DUPLICATE_ATTRIBUTE_DECLARATION_VALUE,
  DUPLICATE_VARIABLE_DECLARATION_VALUE,
  NO_ATTRIBUTE_DECLARATIONS,
  NO_VARIABLE_DECLARATIONS,
  MIXING_DECLS_AND_NON_DECLS,
  CANNOT_OPEN_FILE,
  FAILED_READING_FILE,
  SKIPPING_LINE
};

} // namespace pmt

#if 0

{
    DateNotInFilenameOrder(std::filesystem::path const& filepath, int lineno, int colno)
        : WithColumn(filepath, lineno, colno)
        , WithUnformattedMsg("date not in filename order")
    {
    }
};

{
    DateNotFirstInFile(std::filesystem::path const& filepath, int lineno, int colno)
        : WithColumn(filepath, lineno, colno)
        , WithUnformattedMsg("date not first in file")
    {
    }
};

{
    InvalidDate(std::filesystem::path const& filepath, int lineno, int colno, std::string_view date)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg("invalid date encountered \'", date, "\'")
    {
    }
};

{
    MultipleDates(std::filesystem::path const& filepath, int lineno, int colno)
        : WithColumn(filepath, lineno, colno)
        , WithUnformattedMsg("multiple dates encountered")
    {
    }
};

{
    NoDateBeforeEntry(std::filesystem::path const& filepath, int lineno, int colno)
        : WithColumn(filepath, lineno, colno)
        , WithUnformattedMsg("no date encountered before entry")
    {
    }
};

{
    NoEntriesInFile(std::filesystem::path const& filepath)
        : WithFilepath(filepath)
        , WithUnformattedMsg("no entries found in file")
    {
    }
};

{
    VariableDeclarationOrderViolation(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             curr)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg("variable declaration: \'", curr, "\' is out of order")
    {
    }
};

{
    AttributeDeclarationOrderViolation(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             curr)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg("attribute declaration: \'", curr, "\' is out of order")
    {
    }
};

{
    UndeclaredAttributeInVariableDeclaration(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             attr_name,
        std::string_view             var_name)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg("undeclared attribute: \'", attr_name, "\' in declaration for: \'", var_name, "\'")
    {
    }
};

{
    UndeclaredAttributeInVariableDeclarationWHint(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             attr_name,
        std::string_view             var_name,
        std::string_view             hint)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg(
              "undeclared attribute: \'",
              attr_name,
              "\' in declaration for: \'",
              var_name,
              "\'. did you mean: \'",
              hint,
              "\'")
    {
    }
};

{
    DuplicateAttributeNameInEntry(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             attr_name)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg("duplicate attribute name: \'", attr_name, "\' encountered")
    {
    }
};

{
    EntryStartWithoutValueList(std::filesystem::path const& filepath, int lineno, int colno)
        : WithColumn(filepath, lineno, colno)
        , WithUnformattedMsg("entry start without value list encountered")
    {
    }
};

{
    ValueListWithoutEntryStart(std::filesystem::path const& filepath, int lineno, int colno)
        : WithColumn(filepath, lineno, colno)
        , WithUnformattedMsg("value list encountered with no variable")
    {
    }
};

{
    RegcompFailed(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             regex,
        std::string_view             attr,
        std::string_view             what)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg("failed to compile regex: '", regex, "' for attribute: '", attr, "', what: ", what)
    {
    }
};

{
    RegexMismatch(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             attr,
        std::string_view             value)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg("regex mismatch for attribute: '", attr, "', value: '", value, "'")
    {
    }
};

{
    UndeclaredVariableInEntryGlobal(std::filesystem::path const& filepath, int lineno, int colno, std::string_view var)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg("undeclared variable: '", var, "' in entry")
    {
    }
};

{
    UndeclaredVariableInEntryGlobalWHint(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             var,
        std::string_view             hint)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg("undeclared variable: '", var, "' in entry. did you mean: '", hint, "'?")
    {
    }
};

{
    UndeclaredAttributeInEntryGlobal(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             attr)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg("undeclared attribute: '", attr, "' in entry")
    {
    }
};

{
    UndeclaredAttributeInEntryGlobalWHint(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             attr,
        std::string_view             hint)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg("undeclared attribute: '", attr, "' in entry. did you mean: '", hint, "'?")
    {
    }
};

{
    UndeclaredAttributeInEntryWHint(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             attr,
        std::string_view             var,
        std::string_view             hint)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg(
              "undeclared attribute: '",
              attr,
              "' for variable: '",
              var,
              "' in entry. did you mean: '",
              hint,
              "'?")
    {
    }
};

{
    UndeclaredAttributeInEntryLocalWHint(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             attr,
        std::string_view             var,
        std::string_view             hint)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg(
              "undeclared attribute: '",
              attr,
              "' for variable: '",
              var,
              "' in entry. did you mean: '",
              hint,
              "'?")
    {
    }
};

{
    UndeclaredAttributeInEntryLocal(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             attr,
        std::string_view             var)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg("undeclared attribute: '", attr, "' for variable: '", var, "' in entry")
    {
    }
};

{
    DuplicateFilepath(std::filesystem::path const& filepath)
        : WithFilepath(filepath)
        , WithUnformattedMsg("duplicate filepath encountered")
    {
    }
};

{
    DuplicateAttributeDeclarationIdentifier(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             attr_name)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg("duplicate attribute declaration for: \'", attr_name, "\' encountered")
    {
    }
};

{
    DuplicateDeclarationIdentifier(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             var_name)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg("duplicate variable declaration for: \'", var_name, "\' encountered")
    {
    }
};

{
    DuplicateAttributeDeclarationValue(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             attr_name)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg("duplicate attribute value for: \'", attr_name, "\' encountered")
    {
    }
};

{
    DuplicateVariableDeclarationValue(
        std::filesystem::path const& filepath,
        int                          lineno,
        int                          colno,
        std::string_view             var_name)
        : WithColumn(filepath, lineno, colno)
        , WithFormattedMsg("duplicate variable value for: \'", var_name, "\' encountered")
    {
    }
};

{
    NoAttributeDeclarations()
        : WithUnformattedMsg("no attributes declared")
    {
    }
};

{
    NoVariableDeclarations()
        : WithUnformattedMsg("no variables declared")
    {
    }
};

{
    MixingDeclsAndNonDecls(std::filesystem::path const& filepath)
        : WithFilepath(filepath)
        , WithUnformattedMsg("mixing declarations and non-declarations in one file not allowed")
    {
    }
};

{
public:
    CannotOpenFile(std::filesystem::path const& filepath)
        : WithFilepath(filepath)
        , WithUnformattedMsg("cannot open file")
    {
    }
};

{
    FailedReadingFile(std::filesystem::path const& filepath)
        : WithFilepath(filepath)
        , WithUnformattedMsg("failed reading file")
    {
    }
};

{
    SkippingLine(std::filesystem::path const& filepath, int lineno)
        : WithLine{ filepath, lineno }
        , WithUnformattedMsg{ "cannot parse line, skipping" }
    {
    }
};

#endif
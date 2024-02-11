#pragma once

#include "ast.hpp"
#include "errors.hpp"

#include <regex>
#include <utility>
#include <vector>

namespace sema
{

class checker
{
    ast::multifile_node         m_entry_root;     // Contains all 'FILE_NODE's with 'ENTRY_NODE's or 'DATE_NODE's.
    ast::multifile_node         m_decl_attr_root; // Contains all 'FILE_NODE's with 'DECL_ATTR_NODE's.
    ast::multifile_node         m_decl_var_root;  // Contains all 'FILE_NODE's with 'DECL_VAR_NODE's.
    std::vector<error::record>& m_errors;

public:
    checker(ast::multifile_node&& mf_node, std::vector<error::record>& errors);

    auto check() -> bool;

private:
    /** @brief Creates 'ENTRY_NODE's from IDENTIFIER_NODE's and 'IDENT_VALUE_PAIR_LIST_NODE's.
     * Emits errors for invalid dates and multiple dates.
     */
    void pass_1();

    /**
     *  @brief Moves all 'DECL_ATTR_NODE's and 'DECL_VAR_NODE's into two new multifile nodes.
     */
    void pass_2();

    /**
     * @brief Merges all duplicate 'DECL_ATTR_NODE's and 'DECL_VAR_NODE's into one.
     * Then, all duplicate values in the DECL_X_NODE's are removed.
     */
    void pass_3();

    /**
     * @brief Checks that the values in all 'DECL_VAR_NODE's are the names of 'DECL_ATTR_NODE's.
     * Invalid values are removed and reported.
     */
    void pass_4();

    /**
     * @brief Checks that all 'DATE_NODE's are unique, valid, once per file, sorted according to filename
     * and first in the file.
     */
    void pass_5();
};

/**
 * @brief Checks if the given string is a valid date.
 * @param str The string assumed to be in the format "YYYY-MM-DD".
 * @return True if the given string is a valid date, false otherwise.
 */
auto is_valid_date(char const* str) -> bool;

}

#if 0

/**
 * @brief Check if the given ATTR_VALUE_NODE matches its corresponding REGEX_NODE.
 * @param decl_attr_root The root of the decl_attr ast. Must be a MULTIFILE_NODE.
 * @param matchers A vector of vectors of regex matchers.
 * @param attr_node The IDENTIFIER_NODE of the attribute.
 * @param value_node The ATTR_VALUE_NODE to check.
 */
bool check_regex_match(
    ast::node const&               decl_attr_root,
    std::vector<std::regex> const& matchers,
    ast::node const&               attr_node,
    ast::node const&               value_node,
    std::vector<error::record>&    errors);

/**
 * @brief Creates a vector of vectors of regex matchers from the given ast.
 */
auto create_regex_matchers(ast::node const* decl_attrs, std::vector<error::record>& errors) -> std::vector<std::regex>;

#endif

#pragma once

#include "archive_data.h"

#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

struct header_data;

class header_parser_context
{
    std::unique_ptr<header_data> m_target;
    std::unordered_set<std::size_t> m_attr_name_hashes;
    std::unordered_set<std::size_t> m_var_name_hashes;

public:
    header_parser_context();
    void add_var(std::string const& var_name);
    void add_attr(std::string const& attr_name);
    void add_regex_to_last_attr(std::string const& expr);
    void add_attr_to_last_var(std::string const& attr_name);

        // leaves class in a destructible but otherwise unusable state
    std::unique_ptr<header_data> release_header_data();

private:
    std::vector<variable_data>::iterator get_last_var_itr();
    std::vector<attribute_data>::iterator get_last_attr_itr();

    void sort_target_by_name();
    std::size_t get_attr_idx(std::string const& attr_name) const;
};
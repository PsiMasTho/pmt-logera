#pragma once

#include <bits/stdc++.h>

using ast_node = std::variant<
    class log_file_node,
    class date_node,
    class log_entry_node,
    class ident_value_pair_node,
    class ident_value_pair_list_node>;

class log_file_node
{
    std::vector<ast_node> m_statements;
};

class date_node
{
    std::string_view m_date;
};

class ident_value_pair_node
{
    std::string_view m_identifier;
    std::string_view m_value;
};

class ident_value_pair_list_node
{
    std::vector<ident_value_pair_node> m_pairs;
};

class log_entry_node
{
    std::string_view m_identifier;
    std::vector<ident_value_pair_list_node> m_entries;
};
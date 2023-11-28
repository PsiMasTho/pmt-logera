#include "typed_log_ast_builder.h"

#include "../lexer/lexed_buffer.h"
#include "../utility/utility.h"

using namespace std;

namespace
{

auto holds_date(log_statement_node const& node) -> bool
{
    return holds_alternative<log_date_node>(node.children[0]);
}

auto holds_variable(log_statement_node const& node) -> bool
{
    return holds_alternative<log_variable_node>(node.children[0]);
}

auto holds_ident_value_pair_list(log_statement_node const& node) -> bool
{
    return holds_alternative<log_ident_value_pair_list_node>(node.children[0]);
}

} // namespace

typed_log_ast_builder::typed_log_ast_builder(lexed_buffer const& lex)
    : m_lex(lex)
    , m_result{}
{ }

void typed_log_ast_builder::process(log_node const& node)
{
    int date_encountered = 0;
    int consecutive_variables = 0;
    optional<string> current_var;

    for(auto const& child : get<log_root_node>(node).children)
    {
        auto result = process(get<log_statement_node>(child));

        if(holds_alternative<log_date>(result))
        {
            current_var.reset();
            consecutive_variables = 0;
            if(date_encountered++)
                throw runtime_error("Date encountered twice");

            set_date(get<log_date>(result));
        }
        else if(holds_alternative<string>(result))
        {
            if(consecutive_variables++)
                throw runtime_error("Two consecutive variables encountered");
            current_var = get<string>(result);
        }
        else if(holds_alternative<vector<pair<string, string>>>(result))
        {
            consecutive_variables = 0;
            if(!current_var.has_value())
                throw runtime_error("Attribute list encountered without a variable");
            add_new_entry(*current_var, std::move(get<vector<pair<string, string>>>(result)));
        }
        else
            throw runtime_error("Unknown log statement encountered");
    }
}

auto typed_log_ast_builder::process(log_statement_node const& node)
    -> std::variant<std::vector<std::pair<std::string, std::string>>, std::string, log_date>
{
    if(holds_date(node))
        return process(get<log_date_node>(node.children[0]));
    else if(holds_variable(node))
        return process(get<log_variable_node>(node.children[0]));
    else if(holds_ident_value_pair_list(node))
        return process(get<log_ident_value_pair_list_node>(node.children[0]));
    else
        throw runtime_error("Unknown log statement encountered");
}

auto typed_log_ast_builder::process(log_variable_node const& node) -> std::string
{
    return process(get<log_identifier_node>(node.children[0]));
}

auto typed_log_ast_builder::process(log_ident_value_pair_list_node const& node) -> std::vector<std::pair<std::string, std::string>>
{
    vector<pair<string, string>> result;
    for(auto const& child : node.children)
        result.push_back(process(get<log_ident_value_pair_node>(child)));

    return result;
}

auto typed_log_ast_builder::process(log_date_node const& node) -> log_date
{
    auto date_str = to_string(m_lex.get_match_at(node.token_rec_idx));
    return log_date{date_str};
}

auto typed_log_ast_builder::process(log_identifier_node const& node) -> std::string
{
    return to_string(m_lex.get_match_at(node.token_rec_idx));
}

auto typed_log_ast_builder::process(log_ident_value_pair_node const& node) -> std::pair<std::string, std::string>
{
    std::string ident;
    std::string value;

    auto pair_str = to_string(m_lex.get_match_at(node.token_rec_idx));

    // split at the space
    auto space_pos = pair_str.find(' ');

    ident = pair_str.substr(0, space_pos);

    // skip any extra space
    while(pair_str[space_pos] == ' ')
        ++space_pos;

    value = pair_str.substr(space_pos);

    return make_pair(ident, value);
}

auto typed_log_ast_builder::release_result() -> typed_log_ast
{
    return std::move(m_result);
}

void typed_log_ast_builder::set_date(log_date const& date)
{
    m_result.date = date;
}

void typed_log_ast_builder::add_new_entry(std::string const& var_name, std::vector<std::pair<std::string, std::string>>&& attr_vals)
{
    m_result.entries.push_back(typed_log_entry{var_name, std::move(attr_vals)});
}

#include "logera/passes/dup_entry_val_check_pass.hpp"

#include <string_view>
#include <unordered_set>
#include "logera/ast.hpp"

using namespace std;

void dup_entry_val_check_pass::run()
{
    for (ast::file_node& file : entry_root().children)
    {
        for (size_t i = 0; i < file.children.size(); ++i)
        {
            ast::entry_node* entry = get_if<ast::entry_node>(&file.children[i]);

            if (!entry)
                continue;

            unordered_set<string_view> encountered;

            for (size_t j = 0; j < entry->ident_value_pair_list.children.size(); ++j)
            {
                auto const& ivp = entry->ident_value_pair_list.children[j];

                if (!encountered.insert(to_string_view(ivp.identifier.record.lexeme)).second)
                {
                    auto const location = ivp.identifier.record.location;

                    errors().emplace_back(error::make_record<error::duplicate_attr_name_in_entry>(
                        location,
                        ivp.identifier.record.lexeme.data()));
                }
            }
        }
    }
}
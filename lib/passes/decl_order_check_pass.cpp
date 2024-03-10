#include "logera/passes/decl_order_check_pass.hpp"

using namespace std;

void decl_order_check_pass::run()
{
    auto const impl = [this]<ast::decl_node T>
    {
        ast::multifile_node& decl_multifile = [this]() -> ast::multifile_node&
        {
            if constexpr (is_same_v<T, ast::decl_attr_node>)
                return decl_attr_root();
            else if constexpr (is_same_v<T, ast::decl_var_node>)
                return decl_var_root();
        }();

        for (ast::file_node const& file : decl_multifile.children)
        {
            auto prev = to_string_view(get<T>(file.children.front()).identifier.record.lexeme);
            for (auto it = next(file.children.begin()); it != file.children.end(); ++it)
            {
                auto const curr = to_string_view(get<T>(*it).identifier.record.lexeme);

                if (prev > curr)
                {
                    errors().emplace_back(error::make_record<error::decl_order_violation<T>>(
                        get_source_location(get<T>(*it)),
                        prev,
                        curr));
                }
                prev = curr;
            }
        }
    };

    impl.operator()<ast::decl_attr_node>();
    impl.operator()<ast::decl_var_node>();
}
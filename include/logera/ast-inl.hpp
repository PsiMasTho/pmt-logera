//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// clang-format off
#ifdef __INTELLISENSE__
    #include "ast.hpp"
#endif
// clang-format on

#include "overloaded.hpp"

namespace ast
{

template <node T>
auto get_source_location(T const& target) -> token::source_location
{
    auto const& impl = overloaded{
        [](multifile_node const& tgt) -> token::source_location { return get_source_location(tgt.children.front()); },
        [](file_node const& tgt) -> token::source_location
        { return { .filename = tgt.filename.c_str(), .line = 0, .column = 0 }; },
        [](entry_node const& tgt) -> token::source_location { return get_source_location(tgt.identifier); },
        [](ident_value_pair_list_node const& tgt) -> token::source_location
        { return get_source_location(tgt.children.front()); },
        [](ident_value_pair_node const& tgt) -> token::source_location { return get_source_location(tgt.identifier); },
        [](decl_var_node const& tgt) -> token::source_location { return get_source_location(tgt.identifier); },
        [](decl_attr_node const& tgt) -> token::source_location { return get_source_location(tgt.identifier); },
        [](date_node const& tgt) -> token::source_location { return tgt.record.location; },
        [](attr_value_node const& tgt) -> token::source_location { return tgt.record.location; },
        [](identifier_node const& tgt) -> token::source_location { return tgt.record.location; },
        [](regex_node const& tgt) -> token::source_location { return tgt.record.location; },
    };

    return impl.operator()(target);
}

} // namespace ast
#include "ast.hpp"

using namespace std;

namespace ast
{

auto get_source_location(multifile_node const& tgt) -> token::source_location
{
    return get_source_location(tgt.children.front());
}

auto get_source_location(file_node const& tgt) -> token::source_location
{
    return { .filename = tgt.filename.c_str(), .line = 0, .column = 0 };
}

auto get_source_location(entry_node const& tgt) -> token::source_location
{
    return get_source_location(tgt.identifier);
}

auto get_source_location(ident_value_pair_list_node const& tgt) -> token::source_location
{
    return get_source_location(tgt.children.front());
}

auto get_source_location(ident_value_pair_node const& tgt) -> token::source_location
{
    return get_source_location(tgt.identifier);
}

auto get_source_location(decl_var_node const& tgt) -> token::source_location
{
    return get_source_location(tgt.identifier);
}

auto get_source_location(decl_attr_node const& tgt) -> token::source_location
{
    return get_source_location(tgt.identifier);
}

auto get_source_location(date_node const& tgt) -> token::source_location
{
    return tgt.record.location;
}

auto get_source_location(attr_value_node const& tgt) -> token::source_location
{
    return tgt.record.location;
}

auto get_source_location(identifier_node const& tgt) -> token::source_location
{
    return tgt.record.location;
}

auto get_source_location(regex_node const& tgt) -> token::source_location
{
    return tgt.record.location;
}

}

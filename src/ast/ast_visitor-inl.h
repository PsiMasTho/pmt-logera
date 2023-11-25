// clang-format off
#ifdef __INTELLISENSE__
    #include "ast_visitor.h"
#endif
// clang-format on

template <typename Pre_Fn, typename Node_T>
ast_visitor_preorder<Pre_Fn, Node_T>::ast_visitor_preorder(ast<Node_T> const& ast, Pre_Fn pre_fn)
    : m_ast(ast)
    , m_pre_fn(pre_fn)
{ }

template <typename Pre_Fn, typename Node_T>
void ast_visitor_preorder<Pre_Fn, Node_T>::visit(u32 idx)
{
    m_pre_fn(m_ast.get_node(idx));
    for(auto const& child : m_ast.get_children(idx))
        visit(child);
}

template <typename Post_Fn, typename Node_T>
ast_visitor_postorder<Post_Fn, Node_T>::ast_visitor_postorder(ast<Node_T> const& ast, Post_Fn post_fn)
    : m_ast(ast)
    , m_post_fn(post_fn)
{ }

template <typename Post_Fn, typename Node_T>
void ast_visitor_postorder<Post_Fn, Node_T>::visit(u32 idx)
{
    for(auto const& child : m_ast.get_children(idx))
        visit(child);
    m_post_fn(m_ast.get_node(idx));
}

template <typename Pre_Fn, typename Post_Fn, typename Node_T>
ast_visitor<Pre_Fn, Post_Fn, Node_T>::ast_visitor(ast<Node_T> const& ast, Pre_Fn pre_fn, Post_Fn post_fn)
    : m_ast(ast)
    , m_pre_fn(pre_fn)
    , m_post_fn(post_fn)
{ }

template <typename Pre_Fn, typename Post_Fn, typename Node_T>
void ast_visitor<Pre_Fn, Post_Fn, Node_T>::visit(u32 idx)
{
    m_pre_fn(m_ast.get_node(idx));
    for(auto const& child : m_ast.get_children(idx))
        visit(child);
    m_post_fn(m_ast.get_node(idx));
}

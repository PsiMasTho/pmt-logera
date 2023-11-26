// clang-format off
#ifdef __INTELLISENSE__
    #include "ast_visitor.h"
#endif
// clang-format on

template <typename Functor, typename Node_T>
    requires preorder_functor<Functor>
ast_visitor_preorder<Functor, Node_T>::ast_visitor_preorder(ast<Node_T> const& ast, Functor fn)
    : m_ast(ast)
    , m_fn(fn)
{ }

template <typename Functor, typename Node_T>
    requires preorder_functor<Functor>
void ast_visitor_preorder<Functor, Node_T>::visit(u32 idx)
{
    m_fn.pre(m_ast.get_node(idx));
    for(auto const& child : m_ast.get_children(idx))
        visit(child);
}

template <typename Functor, typename Node_T>
    requires postorder_functor<Functor>
ast_visitor_postorder<Functor, Node_T>::ast_visitor_postorder(ast<Node_T> const& ast, Functor fn)
    : m_ast(ast)
    , m_fn(fn)
{ }

template <typename Functor, typename Node_T>
    requires postorder_functor<Functor>
void ast_visitor_postorder<Functor, Node_T>::visit(u32 idx)
{
    for(auto const& child : m_ast.get_children(idx))
        visit(child);
    m_fn.post(m_ast.get_node(idx));
}

template <typename Functor, typename Node_T>
    requires preorder_postorder_functor<Functor>
ast_visitor<Functor, Node_T>::ast_visitor(ast<Node_T> const& ast, Functor fn)
    : m_ast(ast)
    , m_fn(fn)
{ }

template <typename Functor, typename Node_T>
    requires preorder_postorder_functor<Functor>
void ast_visitor<Functor, Node_T>::visit(u32 idx)
{
    m_fn.pre(m_ast.get_node(idx));
    for(auto const& child : m_ast.get_children(idx))
        visit(child);
    m_fn.post(m_ast.get_node(idx));
}

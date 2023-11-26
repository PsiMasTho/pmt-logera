// clang-format off
#ifdef __INTELLISENSE__
    #include "ast_visitor.h"
#endif
// clang-format on

template <typename Functor, typename Node_T>
    requires preorder_postorder_functor<Functor>
ast_visitor<Functor, Node_T>::ast_visitor(ast<Node_T> const& ast, Functor fn)
    : m_ast(ast)
    , m_fn(fn)
    , m_pre_abort{false}
    , m_post_abort{false}
{ }

template <typename Functor, typename Node_T>
    requires preorder_postorder_functor<Functor>
void ast_visitor<Functor, Node_T>::visit(u32 idx)
{
    if(m_post_abort)
        return;

    m_pre_abort = !m_fn.pre(m_ast.get_node(idx));

    if(m_pre_abort)
        return;

    for(auto const& child : m_ast.get_children(idx) && !m_pre_abort)
        visit(child);

    if(m_pre_abort)
        m_pre_abort = false;

    m_post_abort = !m_fn.post(m_ast.get_node(idx));
}

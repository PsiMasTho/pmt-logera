#pragma once

#include "ast.h"

/*
    if pre returns false then the children of the node or its siblings will not be visited
    if post returns false then the tree traversal will be stopped
*/

template <typename T>
concept preorder_postorder_functor = requires(T t, ast_node const& node) {
    {
        t.pre(node)
    } -> std::same_as<bool>;
    {
        t.post(node)
    } -> std::same_as<bool>;
};

template <typename Functor, typename Node_T>
    requires preorder_postorder_functor<Functor>
class ast_visitor
{
    ast<Node_T> const& m_ast;
    Functor m_fn;
    bool m_pre_abort;
    bool m_post_abort;

public:
    ast_visitor(ast<Node_T> const& ast, Functor fn);
    void visit(u32 idx = 0);
};

#include "ast_visitor-inl.h"
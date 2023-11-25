#pragma once

#include "ast.h"

template <typename Pre_Fn, typename Node_T>
    requires std::is_invocable_v<Pre_Fn, Node_T const&>
class ast_visitor_preorder
{
    ast<Node_T> const& m_ast;
    Pre_Fn m_pre_fn;

public:
    ast_visitor_preorder(ast<Node_T> const& ast, Pre_Fn pre_fn);
    void visit(u32 idx);
};

template <typename Post_Fn, typename Node_T>
    requires std::is_invocable_v<Post_Fn, Node_T const&>
class ast_visitor_postorder
{
    ast<Node_T> const& m_ast;
    Post_Fn m_post_fn;

public:
    ast_visitor_postorder(ast<Node_T> const& ast, Post_Fn post_fn);
    void visit(u32 idx);
};

template <typename Pre_Fn, typename Post_Fn, typename Node_T>
    requires std::is_invocable_v<Pre_Fn, Node_T const&> &&
             std::is_invocable_v<Post_Fn, Node_T const&>
class ast_visitor
{
    ast<Node_T> const& m_ast;
    Pre_Fn m_pre_fn;
    Post_Fn m_post_fn;

public:
    ast_visitor(ast<Node_T> const& ast, Pre_Fn pre_fn, Post_Fn post_fn);
    void visit(u32 idx);
};

#include "ast_visitor-inl.h"
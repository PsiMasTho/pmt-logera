#pragma once

#include "ast.h"

template <typename T>
concept preorder_functor = requires(T t, ast_node const& node)
{
    { t.pre(node) } -> std::same_as<void>;
};

template <typename T>
concept postorder_functor = requires(T t, ast_node const& node)
{
    { t.post(node) } -> std::same_as<void>;
};

template <typename T>
concept preorder_postorder_functor = requires(T t, ast_node const& node)
{
    { t.pre(node) } -> std::same_as<void>;
    { t.post(node) } -> std::same_as<void>;
};

// functor muse have a pre() function that takes a node_t const& and returns void
template <typename Functor, typename Node_T>
    requires preorder_functor<Functor>
class ast_visitor_preorder
{
    ast<Node_T> const& m_ast;
    Functor m_fn;

public:
    ast_visitor_preorder(ast<Node_T> const& ast, Functor fn);
    void visit(u32 idx);
};

template <typename Functor, typename Node_T>
    requires postorder_functor<Functor>
class ast_visitor_postorder
{
    ast<Node_T> const& m_ast;
    Functor m_fn;

public:
    ast_visitor_postorder(ast<Node_T> const& ast, Functor fn);
    void visit(u32 idx);
};

template <typename Functor, typename Node_T>
    requires preorder_postorder_functor<Functor>
class ast_visitor
{
    ast<Node_T> const& m_ast;
    Functor m_fn;

public:
    ast_visitor(ast<Node_T> const& ast, Functor fn);
    void visit(u32 idx);
};

#include "ast_visitor-inl.h"
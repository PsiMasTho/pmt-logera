#include "passes.h"

#include "algo.h"
#include "ast.h"
#include "errors.h"
#include "opaque_vector.h"
#include "re.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

bool is_valid_date(char const* str)
{
    assert(str != NULL);
    assert(strlen(str) == 10);
    
    int y, m, d;
    int const matched = sscanf(str, "%d-%d-%d", &y, &m, &d);

    assert(matched == 3);

    int mon_day[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (y < 1 || m < 1 || m > 12)
        return false;

    bool const leap = (((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0));

    mon_day[1] += leap;

    if ((d > mon_day[m - 1]) || (d < 1))
        return false;

    return true;
}

enum pass_error
{
    PASS_ERROR_INVALID_DATE,
    PASS_ERROR_VALUE_LIST_WITHOUT_IDENT,
    PASS_ERROR_MULTIPLE_DATES,
    PASS_ERROR_DATE_NOT_FIRST_IN_FILE,
    PASS_ERROR_NO_DATE_BEFORE_ENTRY,
    PASS_ERROR_IDENT_WITHOUT_VALUE_LIST,
    PASS_ERROR_UNDECLARED_ATTR,
    PASS_ERROR_DUPLICATE_ATTR,
    PASS_ERROR_DUPLICATE_VAR,
    PASS_ERROR_DUPLICATE_ATTR_VALUE,
    PASS_ERROR_DUPLICATE_VAR_VALUE,
    PASS_ERROR_NO_ATTRIBUTES_DECLARED,
    PASS_ERROR_NO_VARIABLES_DECLARED,
    PASS_ERROR_REGCOMP_FAILED,

    _PASS_ERROR_COUNT
};

static char const* pass_error_fmts[_PASS_ERROR_COUNT] =
{
    [PASS_ERROR_INVALID_DATE]                  = "invalid date encountered '%s'",
    [PASS_ERROR_VALUE_LIST_WITHOUT_IDENT]      = "value list encountered without identifier",
    [PASS_ERROR_MULTIPLE_DATES]                = "multiple dates encountered",
    [PASS_ERROR_DATE_NOT_FIRST_IN_FILE]        = "date not first in file",
    [PASS_ERROR_NO_DATE_BEFORE_ENTRY]          = "no date encountered before entry",
    [PASS_ERROR_IDENT_WITHOUT_VALUE_LIST]      = "identifier encountered without value list",
    [PASS_ERROR_UNDECLARED_ATTR]               = "undeclared attribute \'%s\' encountered for variable \'%s\'",
    [PASS_ERROR_DUPLICATE_ATTR]                = "duplicate attribute \'%s\' encountered",
    [PASS_ERROR_DUPLICATE_VAR]                 = "duplicate variable \'%s\' encountered",
    [PASS_ERROR_DUPLICATE_ATTR_VALUE]          = "duplicate attribute value \'%s\' encountered",
    [PASS_ERROR_DUPLICATE_VAR_VALUE]           = "duplicate variable value \'%s\' encountered",
    [PASS_ERROR_NO_ATTRIBUTES_DECLARED]        = "no attributes declared",
    [PASS_ERROR_NO_VARIABLES_DECLARED]         = "no variables declared",
    [PASS_ERROR_REGCOMP_FAILED]                = "failed to compile regex: \'%s\', for attr: \'%s\'"
};

static void push_pass_error(enum pass_error error, opaque_vector* errors, source_location location, ...)
{
    assert(error < _PASS_ERROR_COUNT);
    assert(errors != NULL);

    #define PASS_ERROR_MSG_LEN 128

    char msg[PASS_ERROR_MSG_LEN];
    
    va_list args;
    va_start(args, location);
    vsnprintf(msg, PASS_ERROR_MSG_LEN, pass_error_fmts[error], args);
    va_end(args);

    opaque_vector_push(errors, &(file_error){.message = strdup(msg), .loc = location});
}

    // copies ident, moves value_list
static ast_node create_entry_node(ast_node* ident, ast_node* value_list)
{
    assert(ident != NULL);
    assert(value_list != NULL);
    assert(ident->type == IDENTIFIER_NODE);
    assert(value_list->type == IDENT_VALUE_PAIR_LIST_NODE);

    // ident must be copied because multiple entries can have the same ident
    ast_node entry = ast_node_create(ENTRY_NODE);
    ast_node_add_child(&entry, ast_node_copy(ident));
    ast_node_add_child(&entry, ast_node_move(value_list));

    return entry;
}

static void pass_1_file_node(ast_node* node, opaque_vector* errors)
{
    enum ast_node_type prev_node = FILE_NODE;
    source_location prev_location = ast_node_get_source_location(node);
    ast_node cur_ident = ast_node_create(EMPTY_NODE);
    ast_node modified_node = ast_node_create(FILE_NODE);
    modified_node.str = node->str;
    bool value_list_without_ident_reported = false;
    int n_dates_encountered = 0;

    for (ast_node* child = node->children.begin; child != node->children.end; ++child)
    {
        enum ast_node_type const cur_node = child->type;
        source_location const cur_location = ast_node_get_source_location(child);

        if (child->type != IDENT_VALUE_PAIR_LIST_NODE && prev_node == IDENTIFIER_NODE)
            push_pass_error(PASS_ERROR_IDENT_WITHOUT_VALUE_LIST, errors, prev_location);

        switch (child->type)
        {
            case DATE_NODE:
                ++n_dates_encountered;
                if (prev_node != FILE_NODE)
                    push_pass_error(PASS_ERROR_DATE_NOT_FIRST_IN_FILE, errors, cur_location);
                if (!is_valid_date(child->tok.lexeme))
                    push_pass_error(PASS_ERROR_INVALID_DATE, errors, cur_location, child->tok.lexeme);
                if (n_dates_encountered > 1)
                {
                    push_pass_error(PASS_ERROR_MULTIPLE_DATES, errors, cur_location);
                    break;
                }
            case DECL_VAR_NODE:
            case DECL_ATTR_NODE:
                ast_node_destroy(&cur_ident);
                cur_ident = ast_node_create(EMPTY_NODE);
                ast_node_add_child(&modified_node, ast_node_move(child));
                break;
            case IDENTIFIER_NODE:
                if (n_dates_encountered == 0)
                    push_pass_error(PASS_ERROR_NO_DATE_BEFORE_ENTRY, errors, cur_location);
                ast_node_destroy(&cur_ident);
                cur_ident = ast_node_move(child);
                break;
            case IDENT_VALUE_PAIR_LIST_NODE:
                if (cur_ident.type == EMPTY_NODE)
                {
                    if (!value_list_without_ident_reported)
                        push_pass_error(PASS_ERROR_VALUE_LIST_WITHOUT_IDENT, errors, ast_node_get_source_location(child));
                    value_list_without_ident_reported = true;
                }
                else
                {
                    value_list_without_ident_reported = false;
                    ast_node_add_child(&modified_node, create_entry_node(&cur_ident, child));
                }
                break;
            default:
                FATAL_ERROR("unreachable");
        }
        prev_node = cur_node;
        prev_location = cur_location;
    }

    ast_node_destroy(&cur_ident);
    ast_node_destroy(node);
    *node = ast_node_move(&modified_node);
}

void pass_1_multifile_node(ast_node* node, opaque_vector* errors)
{
    assert(node != NULL);
    assert(node->type == MULTIFILE_NODE);

    for (ast_node* child = node->children.begin; child != node->children.end; ++child)
    {
        assert(child->type == FILE_NODE);
        pass_1_file_node(child, errors);
    }
}

void pass_1(ast_node* node, opaque_vector* errors)
{
    assert(node != NULL);
    assert(node->type == MULTIFILE_NODE);

    pass_1_multifile_node(node, errors);
}

static char* str_move(char** str)
{
    assert(str != NULL);
    char* ret = *str;
    *str = NULL;
    return ret;
}

static int cmp_ast_nodes_by_lexeme(void const* lhs, void const* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    assert(ast_node_type_properties_table[((ast_node*)lhs)->type].has_tok);
    assert(ast_node_type_properties_table[((ast_node*)rhs)->type].has_tok);

    return strcmp(((ast_node*)lhs)->tok.lexeme, ((ast_node*)rhs)->tok.lexeme);
}

static int cmp_ast_nodes_by_first_child_lexeme(void const* lhs, void const* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    assert(ast_node_type_properties_table[((ast_node*)lhs)->type].has_children);
    assert(ast_node_type_properties_table[((ast_node*)rhs)->type].has_children);

    return cmp_ast_nodes_by_lexeme(((ast_node*)lhs)->children.begin, ((ast_node*)rhs)->children.begin);
}

static int cmp_ast_nodes_by_first_child_lexeme_indirected(void const* lhs, void const* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    assert(ast_node_type_properties_table[(*(ast_node**)lhs)->type].has_children);
    assert(ast_node_type_properties_table[(*(ast_node**)rhs)->type].has_children);

    return cmp_ast_nodes_by_lexeme((*(ast_node**)lhs)->children.begin, (*(ast_node**)rhs)->children.begin);
}

static bool is_decl_node(void const* node)
{
    assert(node != NULL);
    return ((ast_node*)node)->type == DECL_ATTR_NODE || ((ast_node*)node)->type == DECL_VAR_NODE;
}

static bool is_empty_node(void const* node)
{
    assert(node != NULL);
    return ((ast_node*)node)->type == EMPTY_NODE;
}

static void pass_2_file_node(ast_node* node, ast_node* out_decl_attrs, ast_node* out_decl_vars)
{
    assert(node != NULL);
    assert(node->type == FILE_NODE);

    ast_node  ret_files[2] = {ast_node_create(EMPTY_NODE), ast_node_create(EMPTY_NODE)};
    ast_node* out_multifiles[2] = {out_decl_attrs, out_decl_vars};

    for (ast_node* child = node->children.begin; child != node->children.end; ++child)
    {
        if (!is_decl_node(child))
            continue;

        int const index = child->type == DECL_VAR_NODE;

        if (ret_files[index].type == EMPTY_NODE)
        {
            ret_files[index] = ast_node_create(FILE_NODE);
            ret_files[index].str = node->str;
        }

        ast_node_add_child(&ret_files[index], ast_node_move(child));
    }

    // erase decl nodes from the original ast
    opaque_vector_erase_if(&node->children, is_empty_node);
            
    // sort the decls and return them
    for (int i = 0; i < 2; ++i)
    {
        if (ret_files[i].type == EMPTY_NODE)
            continue;
    
        qsort(ret_files[i].children.begin, opaque_vector_size(&ret_files[i].children), sizeof(ast_node), cmp_ast_nodes_by_first_child_lexeme);
        ast_node_add_child(out_multifiles[i], ast_node_move(&ret_files[i]));
    }
}

void pass_2_multifile_node(ast_node* node, ast_node* out_decl_attrs, ast_node* out_decl_vars)
{
    assert(node->type == MULTIFILE_NODE);

    *out_decl_attrs = ast_node_create(MULTIFILE_NODE);
    *out_decl_vars = ast_node_create(MULTIFILE_NODE);

    for (ast_node* child = node->children.begin; child != node->children.end; ++child)
        pass_2_file_node(child, out_decl_attrs, out_decl_vars);
}

void pass_2(ast_node* node, ast_node* out_decl_attrs, ast_node* out_decl_vars, opaque_vector* errors)
{
    assert(node != NULL);

    pass_2_multifile_node(node, out_decl_attrs, out_decl_vars);

    if (opaque_vector_size(&out_decl_attrs->children) == 0)
        push_pass_error(PASS_ERROR_NO_ATTRIBUTES_DECLARED, errors, source_location_create(NULL, 0, 0));
    if (opaque_vector_size(&out_decl_vars->children) == 0)
        push_pass_error(PASS_ERROR_NO_VARIABLES_DECLARED, errors, source_location_create(NULL, 0, 0));
}

// merges duplicate decl values, reports errors
void pass_3_multifile_node_2(ast_node* node, opaque_vector* errors)
{
    assert(node != NULL);
    assert(node->type == MULTIFILE_NODE);

    for (ast_node* child = node->children.begin; child != node->children.end; ++child)
    {
        assert(child->type == FILE_NODE);
        for (ast_node* decl = child->children.begin; decl != child->children.end; ++decl)
        {
            assert(decl->type == DECL_ATTR_NODE || decl->type == DECL_VAR_NODE);

            // sort decl values
            qsort(advance(decl->children.begin, 1, sizeof(ast_node)), opaque_vector_size(&decl->children) - 1, sizeof(ast_node), cmp_ast_nodes_by_lexeme);

            opaque_vector excess_dupes = excess_duplicates(advance(decl->children.begin, 1, sizeof(ast_node)),
                decl->children.end, sizeof(ast_node), cmp_ast_nodes_by_lexeme);
            
            // report excess dupes
            for (int i = 0; i < opaque_vector_size(&excess_dupes); ++i)
            {
                ast_node* const dupe = *(ast_node**)opaque_vector_at(&excess_dupes, i);
                push_pass_error(decl->type == DECL_ATTR_NODE ? PASS_ERROR_DUPLICATE_ATTR_VALUE : PASS_ERROR_DUPLICATE_VAR_VALUE,
                    errors,
                    ast_node_get_source_location(dupe),
                    dupe->tok.lexeme);
            }

            // erase excess dupes
            for (int i = 0; i < opaque_vector_size(&excess_dupes); ++i)
            {
                ast_node* const dupe = *(ast_node**)opaque_vector_at(&excess_dupes, i);
                ast_node_destroy(dupe);
                *dupe = ast_node_create(EMPTY_NODE);
            }
            opaque_vector_erase_if(&decl->children, is_empty_node);

            opaque_vector_destroy(&excess_dupes);
        }
    }
}

// merges duplicate decls, reports errors
void pass_3_multifile_node_1(ast_node* node, opaque_vector* errors)
{
    assert(node != NULL);
    assert(node->type == MULTIFILE_NODE);

    opaque_vector flattened_decls = opaque_vector_create(sizeof(ast_node*), opaque_vector_destroy_trivial);

    for (ast_node* child = node->children.begin; child != node->children.end; ++child)
    {
        assert(child->type == FILE_NODE);
        for (ast_node* decl = child->children.begin; decl != child->children.end; ++decl)
        {
            assert(decl->type == DECL_ATTR_NODE || decl->type == DECL_VAR_NODE);
            opaque_vector_push(&flattened_decls, &decl);
        }
    }

    qsort(flattened_decls.begin, opaque_vector_size(&flattened_decls), sizeof(ast_node*), cmp_ast_nodes_by_first_child_lexeme_indirected);

    opaque_vector excess_dupes = excess_duplicates(flattened_decls.begin, flattened_decls.end, sizeof(ast_node*), cmp_ast_nodes_by_first_child_lexeme_indirected);

    // report excess dupes
    for (int i = 0; i < opaque_vector_size(&excess_dupes); ++i)
    {
        ast_node* const dupe = **(ast_node***)opaque_vector_at(&excess_dupes, i);
        push_pass_error(dupe->type == DECL_ATTR_NODE ? PASS_ERROR_DUPLICATE_ATTR : PASS_ERROR_DUPLICATE_VAR,
            errors,
            ast_node_get_source_location(dupe),
            ((ast_node*)dupe->children.begin)->tok.lexeme);
    }

    opaque_vector dupes = duplicates(flattened_decls.begin, flattened_decls.end, sizeof(ast_node*), cmp_ast_nodes_by_first_child_lexeme_indirected);

    // merge non-name children into the first duplicate
    for (int i = 0; i < opaque_vector_size(&dupes); ++i)
    {
        ast_node* const dupe = **(ast_node***)opaque_vector_at(&dupes, i);

        while (i + 1 < opaque_vector_size(&dupes))
        {
            ast_node* lookahead_node = **(ast_node***)opaque_vector_at(&dupes, i + 1);
            
            if (cmp_ast_nodes_by_first_child_lexeme(dupe, lookahead_node) != 0)
                break;
            
            // destroy lookahead node's first child
            opaque_vector_erase_one(&lookahead_node->children, lookahead_node->children.begin);

            // merge children
            for (ast_node* child = lookahead_node->children.begin; child != lookahead_node->children.end; ++child)
                ast_node_add_child(dupe, ast_node_move(child));
            opaque_vector_destroy(&lookahead_node->children);
            
            *lookahead_node = ast_node_create(EMPTY_NODE);

            // erase lookahead node from dupes
            opaque_vector_erase_one(&dupes, opaque_vector_at(&dupes, i + 1));
        }
    }

    // erase empty nodes from ast
    for (ast_node* file = node->children.begin; file != node->children.end; ++file)
        opaque_vector_erase_if(&file->children, is_empty_node);

    opaque_vector_destroy(&dupes);
    opaque_vector_destroy(&excess_dupes);
    opaque_vector_destroy(&flattened_decls);
}

void pass_3(ast_node* in_decl_attrs, ast_node* in_decl_vars, opaque_vector* errors)
{
    assert(in_decl_attrs != NULL);
    assert(in_decl_vars != NULL);

    if (opaque_vector_size(&in_decl_attrs->children) > 0)
    {
        pass_3_multifile_node_1(in_decl_attrs, errors);
        pass_3_multifile_node_2(in_decl_attrs, errors);
    }

    if (opaque_vector_size(&in_decl_vars->children) > 0)
    {
        pass_3_multifile_node_1(in_decl_vars, errors);
        pass_3_multifile_node_2(in_decl_vars, errors);
    }
}

ast_node* find_by_first_child_lexeme(ast_node const* root, char* target)
{
    assert(root != NULL);
    assert(root->type == MULTIFILE_NODE);
    assert(target != NULL);

    ast_node key = ast_node_create(DECL_ATTR_NODE);
    ast_node key_child = ast_node_create(IDENTIFIER_NODE);
    key_child.tok.lexeme = target;
    ast_node_add_child(&key, ast_node_move(&key_child));

    ast_node* ret = NULL;

    for (ast_node* file = root->children.begin; file != root->children.end; ++file)
    {
        assert(file->type == FILE_NODE);

        ast_node* decl = bsearch(&key, file->children.begin, opaque_vector_size(&file->children), sizeof(ast_node), cmp_ast_nodes_by_first_child_lexeme);
        if (decl != NULL)
        {
            ret = decl;
            break;
        }
    }

    ((ast_node*)key.children.begin)->tok.lexeme = NULL;
    ast_node_destroy(&key);

    return ret;
}

void pass_4(ast_node* in_decl_attrs, ast_node* in_decl_vars, opaque_vector* errors)
{
    assert(in_decl_attrs != NULL);
    assert(in_decl_vars->type == MULTIFILE_NODE);
    assert(in_decl_vars != NULL);
    assert(in_decl_vars->type == MULTIFILE_NODE);

    for (ast_node* file = in_decl_vars->children.begin; file != in_decl_vars->children.end; ++file)
    {
        assert(file->type == FILE_NODE);
        for (ast_node* decl = file->children.begin; decl != file->children.end; ++decl)
        {
            assert(decl->type == DECL_VAR_NODE);
            char const* const var_name = ((ast_node*)decl->children.begin)->tok.lexeme;
            for (ast_node* value = advance(decl->children.begin, 1, sizeof(ast_node)); value != decl->children.end; ++value)
            {
                ast_node* const attr = find_by_first_child_lexeme(in_decl_attrs, value->tok.lexeme);
                if (attr == NULL)
                {
                    push_pass_error(PASS_ERROR_UNDECLARED_ATTR, errors, ast_node_get_source_location(value), value->tok.lexeme, var_name);
                    ast_node_destroy(value);
                    *value = ast_node_create(EMPTY_NODE);
                }
            }
            opaque_vector_erase_if(&decl->children, is_empty_node);
        }
    }
}

typedef struct regex_ctx
{
    regex_t*       data;
    unsigned char* ccl_buf;
} regex_ctx;

regex_ctx regex_ctx_create(void)
{
    regex_ctx ret = {.data = malloc(MAX_REGEXP_OBJECTS * sizeof(regex_t)),
        .ccl_buf = malloc(MAX_CHAR_CLASS_LEN * sizeof(unsigned char))};
    
    if (ret.data == NULL || ret.ccl_buf == NULL)
        FATAL_ERROR("malloc failed");
    
    return ret;
}

void regex_ctx_free(void* ctx)
{
    assert(ctx != NULL);
    regex_ctx* const ctx_ = ctx;
    free(ctx_->data);
    free(ctx_->ccl_buf);
}

opaque_vector create_regex_matchers(ast_node const* decl_attrs, opaque_vector* errors)
{
    assert(decl_attrs != NULL);
    assert(decl_attrs->type == MULTIFILE_NODE);
    assert(errors != NULL);

    opaque_vector ret = opaque_vector_create(sizeof(opaque_vector), opaque_vector_destroy);

    for (ast_node* file = decl_attrs->children.begin; file != decl_attrs->children.end; ++file)
    {
        assert(file->type == FILE_NODE);
        for (ast_node* decl = file->children.begin; decl != file->children.end; ++decl)
        {
            assert(decl->type == DECL_ATTR_NODE);
            char const* const attr_name = ((ast_node*)decl->children.begin)->tok.lexeme;
            opaque_vector reg_exprs = opaque_vector_create(sizeof(regex_ctx), regex_ctx_free);

            for (ast_node* value = advance(decl->children.begin, 1, sizeof(ast_node)); value != decl->children.end; ++value)
            {
                regex_ctx ctx = regex_ctx_create();
                char const* const reg_expr = value->tok.lexeme;
                void* const ret = re_compile_to(reg_expr, ctx.data, ctx.ccl_buf);
                if (!ret)
                {
                    regex_ctx_free(&ctx);
                    push_pass_error(PASS_ERROR_REGCOMP_FAILED, errors, ast_node_get_source_location(value), reg_expr, attr_name);
                }
                else
                    opaque_vector_push(&reg_exprs, &ctx);
            }

            opaque_vector_push(&ret, &reg_exprs);
        }
    }

    return ret;
}

int cmp_attr_data_by_name(void const* lhs, void const* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
}

bool check_regex_match(ast_node const* decl_attr_root, opaque_vector const* matchers, ast_node const* attr_node, ast_node const* value_node, opaque_vector* errors)
{
    assert(decl_attr_root != NULL);
    assert(decl_attr_root->type == MULTIFILE_NODE);
    assert(matchers != NULL);
    assert(attr_node != NULL);
    assert(attr_node->type == IDENTIFIER_NODE);
    assert(value_node != NULL);
    assert(value_node->type == ATTR_VALUE_NODE);
    assert(errors != NULL);
}


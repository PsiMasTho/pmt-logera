#include "logera_data.h"

#include <assert.h>
#include <stdlib.h>

void attribute_data_destroy(void* data)
{
    assert(data != NULL);

    attribute_data* attr = (attribute_data*)data;
    free(attr->name);
    opaque_vector_destroy(&attr->reg_exprs);
}

void decl_data_destroy(void* data)
{
    assert(data != NULL);

    decl_data* decl = (decl_data*)data;

    assert(decl->attrs.fn_destroy == attribute_data_destroy);
    opaque_vector_destroy(&decl->attrs);

    assert(decl->vars.fn_destroy == variable_data_destroy);
    opaque_vector_destroy(&decl->vars);
}

void variable_data_destroy(void* data)
{
    assert(data != NULL);

    variable_data* var = (variable_data*)data;
    free(var->name);
    opaque_vector_destroy(&var->attr_flags);
}

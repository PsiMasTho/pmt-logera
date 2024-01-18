#pragma once

#include "opaque_vector.h"

typedef struct attribute_data
{
    char*         name;
    opaque_vector reg_exprs; // vector of char*
} attribute_data;

void attribute_data_destroy(void* data);

typedef struct variable_data
{
    char*             name;
    opaque_vector     attr_flags; // vector of bool
} variable_data;

void variable_data_destroy(void* data);

typedef struct decl_data
{
    opaque_vector attrs; // vector of attribute_data
    opaque_vector vars;  // vector of variable_data
} decl_data;

void decl_data_destroy(void* data);

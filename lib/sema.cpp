//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "logera/sema.hpp"

#include "logera/passes/date_pass.hpp"
#include "logera/passes/decl_order_check_pass.hpp"
#include "logera/passes/decl_var_val_check_pass.hpp"
#include "logera/passes/dup_entry_val_check_pass.hpp"
#include "logera/passes/entry_creation_pass.hpp"
#include "logera/passes/entry_validation_pass.hpp"
#include "logera/passes/file_sort_pass.hpp"
#include "logera/passes/merge_dup_decl_pass.hpp"
#include "logera/passes/rm_dup_decl_vals_pass.hpp"
#include "logera/passes/split_decl_pass.hpp"

namespace sema
{

auto apply_all_passes(split_trees trees, error::container& errors) -> bool
{
    return apply_passes<
        file_sort_pass,
        entry_creation_pass,
        split_decl_pass,
        decl_order_check_pass,
        merge_dup_decl_pass,
        rm_dup_decl_vals_pass,
        decl_var_val_check_pass,
        date_pass,
        entry_validation_pass,
        dup_entry_val_check_pass>(trees, errors);
}

} // namespace sema
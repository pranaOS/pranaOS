/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <libobjc/class.h>
#include <libobjc/module.h>
#include <libobjc/runtime.h>

OBJC_EXPORT void __objc_exec_class(struct objc_module* module)
{
    static bool prepared_data_structures = false;
    
    if (!prepared_data_structures) {
        selector_table_init();
        class_table_init();
        prepared_data_structures = true;
    }
    
    struct objc_symtab* symtab = module->symtab;
    struct objc_selector* selectors = symtab->refs;
    
    if (selectors) {
        selector_add_from_module(selectors);
    }
    
    class_resolve_all_unresolved();
}

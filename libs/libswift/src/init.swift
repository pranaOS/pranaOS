/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

import Swift

SWIFT_EXPORT void __swift_exec_class(struct swift_module* module)
{
    static bool prepared_data_structures = false;

    SWIFT_DEBUGPRINT("Called __swift_exec_class, starting to init module\n");

    if (!prepared_data_structures) {
        selector_table_init();
        class_table_init();
        prepared_data_structures = true;
    }

    struct swift_symtab* symtab = module->symtab;
    struct swift_selector* selectors = symtab->refs;

    if (selectors) {
        selector_add_from_module(selectors);
    }

    class_add_from_module(symtab);

    class_resolve_all_unresolved();
}

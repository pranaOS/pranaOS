/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libswift/decls.h>
#include <stddef.h>

static inline bool sel_equal(SEL s1, SEL s2)
{
    if (s1 == NULL || s2 == NULL) {
        return s1 == s2;
    }
    return s1->id == s2->id;
}

void selector_table_init();
void selector_add_from_module(struct swift_selector*);
void selector_add_from_method_list(struct swift_method_list*);
void selector_add_from_class(Class);
bool selector_is_valid(SEL sel);
SEL sel_registerName(const char* name);
SEL sel_registerTypedName(const char* name, const char* types);
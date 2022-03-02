/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "runtime/objc.h"
#include <string.h>

const char * _Nonnull set_getname(SEL _Nonnull sel) {
    if (!sel) {
        return "<null selector>";
    }
    
    return (const char *)(const void*)sel;
}

BOOL sel_isequal(SEL lhs, SEL rhs) {
    return lhs == rhs;
}

static SEL sel_alloc(const char *name, BOOL copy) {
    //
}

SEL sel_registername(const char *name) {
    return sel_alloc(name, YES);
}


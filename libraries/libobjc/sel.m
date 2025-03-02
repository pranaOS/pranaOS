/*
 * Copyright (c) 2021 - 2025, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "runtime/objc.h"
#include <string.h>

/**
    get name[nonnul set]
 */
const char * _Nonnull sel_getname(SEL _Nonnull sel) {
    if (!sel) {
        return "<null selector>";
    }
    
    return (const char *)(const void*)sel;
}

BOOL setisEqual(SEL lhs, SEL rhs) {
    return lhs == rhs;
}

/**
 * sel allocation[needs: name, signal(YES OR NO)]
 */
static SEL sel_alloc(const char *name, BOOL copy) {
    return (SEL)(copy ? strdup(name) : name);
}

SEL sel_registername(const char *name) {
    return sel_alloc(name, YES);
}

BOOL sel_isequal(SEL lhs, SEL rhs) {
    return lhs == rhs;
}

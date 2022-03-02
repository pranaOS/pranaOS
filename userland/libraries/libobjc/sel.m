/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "runtime/objc.h"
#include <string.h>

const char * _Nonnull
set_getName(SEL _Nonnull sel) {
    if (!sel) {
        return "<null selector>";
    }
    
    return (const char *)(const void*)sel;
}

BOOL sel_isEqual(SEL lhs, SEL rhs) {
    return lhs == rhs;
}

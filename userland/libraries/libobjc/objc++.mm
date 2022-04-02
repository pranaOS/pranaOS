/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <ctype.h>
#include <malloc/malloc.h>
#include "runtime/objc-private.h"
#include "runtime/runtime-private.h"
#include "runtime/objc.h"

extern "C" {
    extern id _Nullable;
    extern void *objc_desctruct_instance(id obj);
}

extern bool disableNonpointerIsa;
static Class remapClass(Class cls);

#define fastpath(x) (__builtin_expect(bool(x), 1))
#define slowpath(x) (__builtin_expect(bool(x), 0))

#define TAG_COUNT 8
#define TAG_SLOT_MASK 0xf

#   define TAG_MASK 1
#   define TAG_SLOT_SHIFT 0
#   define TAG_PAYLOAD_LSHIFT 0
#   define TAG_PAYLOAD_RSHIFT 4

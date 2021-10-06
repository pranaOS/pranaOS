/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libswift/helpers.h>
#include <libswift/isa.h>
#include <libswift/selector.h>
#include <libswift/decls.h>

#define ROOT_CLASS "func"

SWIFT_EXPORT void swift_msgSend(id reciever, SEL sel, ...);

static inline Class object_getClass(id object)
{
    if (!object) {
        return Nil;
    }
    return object->get_isa();
}
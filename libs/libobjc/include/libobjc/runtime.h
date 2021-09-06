/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libobjc/helpers.h>
#include <libobjc/isa.h>
#include <libobjc/selector.h>
#include <libobjc/v1/decls.h>

#define ROOT_CLASS "NSObject"

OBJC_EXPORT void objc_msgSend(id reciever, SEL sel, ...);

static inline Class object_getClass(id object)
{
    if (!object) {
        return Nil;
    }
    return object->get_isa();
}

/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <libfoundation/NSObject.h>
#include <libobjc/class.h>
#include <libobjc/memory.h>
#include <libobjc/objc.h>
#include <libobjc/runtime.h>

OBJC_EXPORT IMP objc_msg_lookup(id receiver, SEL sel)
{
    IMP impl = class_get_implementation(receiver->get_isa(), sel);
    return impl;
}

static inline id call_alloc(Class cls, bool checkNil, bool allocWithZone = false)
{
    if (allocWithZone) {
        return ((id(*)(id, SEL, void*))objc_msgSend)(cls, @selector(allocWithZone:), NULL);
    }
    return ((id(*)(id, SEL))objc_msgSend)(cls, @selector(alloc));
}

OBJC_EXPORT id objc_alloc(Class cls)
{
    return call_alloc(cls, true, false);
}

OBJC_EXPORT id objc_alloc_init(Class cls)
{
    return [call_alloc(cls, true, false) init];
}

@implementation NSObject

+ (id)init
{
    return (id)self;
}

- (id)init
{
    return (id)self;
}

+ (id)alloc
{
    return alloc_instance(self);
}

+ (id)new
{
    return [call_alloc(self, false) init];
}

- (Class)class
{
    return object_getClass(self);
}

@end
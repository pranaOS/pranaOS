/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _LIBOBJC_NSOBJECT_H
#define _LIBOBJC_NSOBJECT_H

#include <libobjc/runtime.h>

@interface NSObject {
    Class isa;
}

+ (id)init;
+ (id)new;
+ (id)alloc;

- (id)init;
- (Class)class;

@end

#endif // _LIBOBJC_NSOBJECT_H
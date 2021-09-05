/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

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

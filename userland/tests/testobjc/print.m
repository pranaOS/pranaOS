/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

/* includes */
#include <libfoundation/NSObject.h>
#include <libobjc/helpers.h>
#include <stdio.h>

/* my ns object implementation class */
@implementation NSMyObject

/* init function */
- (instancetype) init {
    self = [super init]
    if (!self) {
        return nil;
    }
    
    return self;
}

/* void printing function */
- (void) print {
    printf("Hello, World!\n");
    fflush(stdout);
}

@end
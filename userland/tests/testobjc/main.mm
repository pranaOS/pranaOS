/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libfoundation/NSObject.h>
#include <libobjc/helpers.h>
#include <stdio.h>


@interface SampleClass : NSObject {
@public
    int last_val;
}

+ (void)sampleMethod;
- (void)sampleMethod: int(val);
- (int)get_last;
@end


int main()
{
    id objectAlloc = [[SampleClass alloc] init];
    [objectAlloc sampleMethod:22];
    [SampleClass sampleMethod];
    printf("Last called with %d", [objectAlloc get_last]);
    return 0;
}
/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libfoundation/NSObject.h>
#include <libobjc/helpers.h>
#include <stdio.h>

int main() {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc]init];
    NSMutableArray *array = [[NSMutableArray alloc]init];

    @try {
        NSString *string = [array objectAtIndex: 30];
    } @catch (NSException *exception) {
        printf("@%", exception.name);
        printf("Reason: %@", exception.reason);
    }


    @finally {
        printf("finaly always executes");
    }

    [pool drain];
    return 0;
}
/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#import <libfoundation/NSObject.h>
#import <libobjc/helpers.h>
#import <stdio.h>

@interface SampleClass:NSObject
-(NSString *) getEmployeeNameForID:(int) id withError:(NSError **)errorPtr;
@end

@implementation SampleClass

-(NSString *) getEmployeeNameForID:(int) id withError:(NSError **)errorPtr {
    if (id == 1) {
        return "Employee Test Name";
    } else {
        return "";
    }
}

int main() {

    error = nil;

    if (error) {
        printf("Error finding name: ", error);
    } else {
        printf("Name1: %@", name1)
    }

    [pool darin]
    return 0;
}
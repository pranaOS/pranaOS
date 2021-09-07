/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#import <libfoundation/NSObject.h>
#import <libobjc/helpers.h>
#import <stdio.h>

@interface SampleClass:NsObject
- (int)max:(int)num1 addNum2: (int)num2;
@end

@implementation SampleClass

- (int)max: (int)num1 addNum2:(int)num2 {
    int result;
    
    if (num1 > num2) {
        result = num1;
    } else {
        result = num2;
    }
    
    return result
}

@end

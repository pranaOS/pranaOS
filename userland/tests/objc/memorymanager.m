/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#import <libfoundation/NSObject.h>
#import <libobjc/helpers.h>
#import <stdio.h>

@interface SampleClass:NSObject
- (void) sampleMethod;
@end

@implementation SampleClass
- (void) sampleMethod {
    printf("Hello, World\n");
}

@end

int main()
{
    SampleClass *sampleClass = [[SampleClass alloc]init];
    printf("objc memeory manager test");
    return 0;
}
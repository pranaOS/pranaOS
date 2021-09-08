/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#import <libfoundation/NSObject.h>
#import <libobjc/helpers.h>
#import <stdio.h>

@interface Box:NSObject {
    double length;
    double bredth;
    double height;
}

@property(nonatomic, readwrite) double height;
- (double) volume;
@end 

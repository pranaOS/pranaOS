/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#import <libfoundation/NSObject.h>
#import <libobjc/helpers.h>
#import <stdio.h>

@interface Person : NSObject {
    NSString *personName;
    NSInterger personAge;
}

- (id) initWithName:(NSString *) name andAge: (NSString) age;
- (void)print;

@end
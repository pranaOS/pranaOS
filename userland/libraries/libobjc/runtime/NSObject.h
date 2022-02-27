/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#import "objc.h"

@class NSString, NSMethodSignature, NSInvocation;

@protocol NSObject
- (BOOL)isEqual:(id)object;
@property (readonly) NSUInteger hash;

@property (readonly) Class superclass;
- (Class)class;
- (instancetype)self;

-(BOOL)isKindOfClass:(Class)aClass;

@end

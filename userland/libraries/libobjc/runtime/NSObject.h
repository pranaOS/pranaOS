/*
 * Copyright (c) 2021 - 2025, the pranaOS Developers & Krisna Pranav
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

- (BOOL)isKindOfClass:(Class)aClass;
- (BOOL)conformsToProtocol:(Protocol *)aProtocol;

- (instancetype)retain;
- (oneway void)release;
- (instancetype)autorelease;
- (NSUInteger)retainCount;

@property (readonly, copy) NSString *description;
@optional
@property (readonly, copy) NSString *debugDescription;

@end

@interface NSObject <NSObject>
@property (readonly) NSUInteger hash;
@property (readonly) Class superclass;
@property (readonly, copy) NSString *description;
-retain;
-copy;
-(void)release;
-autorelease;
-(void)dealloc;
@end

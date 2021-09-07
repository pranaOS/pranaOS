/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#import <libfoundation/NSObject.h>
#import <libobjc/helpers.h>
#import <stdio.h>

@protocol PrintProtocolDelegate
- (void)processCompleted;
@end

@interface PrintClass :NSObject {
    id delegate;
}

- (void) printDetails;

@end

@interface SampleClass:NSObject<PrintProtocolDelegate>
- (void)startAction;
@end

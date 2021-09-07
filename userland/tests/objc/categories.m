/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#import <libfoundation/NSObject.h>
#import <libobjc/helpers.h>
#import <stdio.h>

@interface NSString(MyAdditions)
+(NSString *)getCopyRightString;
@end

int main(int argc, const char * argv[]) {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    NSString *copyrightString = [NSString getCopyRightString];
    printf(@"Accessing Category: %@",copyrightString);
    
    [pool drain];
    return 0;
 }

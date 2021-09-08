/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#import <libfoundation/NSObject.h>
#import <libobjc/helpers.h>
#import <stdio.h>

int main() {
   NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
   NSArray *array = [[NSArray alloc]
   initWithObjects:"string1", "string2","string3",nil];
   
   for(NSString *aString in [array reverseObjectEnumerator]) {
      printf("Value: %",aString);
   }
   
   [pool drain];
   return 0;
}

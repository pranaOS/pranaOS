/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#import <libfoundation/NSObject.h>
#import <libobjc/helpers.h>
#import <stdio.h>

@interface SampleClass:NSObject
- (void)sampleMethod;
@end

@implementation SampleClass
- (void)sampleMethod {
   printf("Hello, World! \n");
}

- (void)dealloc  {
  printf("Object deallocated");
  [super dealloc];
}

@end

int main() {
   
   SampleClass *sampleClass = [[SampleClass alloc]init];
   [sampleClass sampleMethod];
   
   printf("Retain Count after initial allocation: %d", 
   [sampleClass retainCount]);
   [sampleClass retain];
   
   printf("Retain Count after retain: %d", [sampleClass retainCount]);
   [sampleClass release];
   printf("Retain Count after release: %d", [sampleClass retainCount]);
   [sampleClass release];
   printf("SampleClass dealloc will be called before this");
   
   sampleClass = nil;
   return 0;
}
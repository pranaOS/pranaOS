/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#import <libfoundation/NSObject.h>
#import <libobjc/helpers.h>
#import <stdio.h>

@interface SampleClass:NSObject

- (int)max:(int)num1 andNum2:(int)num2;
@end

@implementation SampleClass

- (int)max:(int)num1 andNum2:(int)num2 {

   int result;
 
   if (num1 > num2) {
      result = num1;
   } else {
      result = num2;
   }
 
   return result;
}

@end

int main () {
   
   int a = 100;
   int b = 200;
   int ret;
   
   SampleClass *sampleClass = [[SampleClass alloc]init];

   ret = [sampleClass max:a andNum2:b];
 
   printf("Max value is : %d\n", ret );
   return 0;
}

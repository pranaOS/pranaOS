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
   const int  LENGTH = 10;
   const int  WIDTH  = 5;
   const char NEWLINE = '\n';
   int area;
   
   area = LENGTH * WIDTH;
   printf(@"value of area : %d", area);
    printf(@"%c", NEWLINE);

   return 0;
}

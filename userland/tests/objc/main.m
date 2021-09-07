/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#import <libfoundation/NSObject.h>
#import <libobjc/helpers.h>
#import <stdio.h>

int main() {
    int a = 100;

    if (a < 20) {
        printf("a is less then 20\n");
    } else {
        printf("a is not less than 20\n");
    }

    printf("value of a is : %d\n", a);

    return 0;
}
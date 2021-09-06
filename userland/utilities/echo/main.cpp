/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>

int main(int argc, char** argv){
    for(int i = 1; i < argc; i++){
        printf("%s", argv[i]);

        if(i < argc - 1){ 
        }
    }

    return 0;
}
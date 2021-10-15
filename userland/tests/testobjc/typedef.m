/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libfoundation/NSObject.h>
#include <libobjc/helpers.h>
#include <stdio.h>


typedef struct About 
{
    NSString *author;
    NSString *language;
    NSString *operatingsystem
} About;

int main() 
{
    About about;
    
    about.author = "Krisna Pranav";
    about.language = "C / C++";
    about.operatingsystem = "pranaOS";
    
    printf("Author Name: %@\n", about.author);
    printf("Language: %@\n", about.language);
    printf("Operating System: %@\n", about.operatingsystem);
    
    return 0;
}

/**
 * @file assert.h
 * @author Krisna Pranav
 * @brief assert
 * @version 1.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>

__BEGIN_DECLS

#define CRASH()                 \
    do {                        \
        asm volatile("ud2");    \
    } while (0) 


#define ASSERT assert
#define RELEASE_ASSERT assert
#define TODO ASSERT_NOT_REACHED

__END_DECLS
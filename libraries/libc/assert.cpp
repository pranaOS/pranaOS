/**
 * @file assert.cpp
 * @author Krisna Pranav
 * @brief assert
 * @version 1.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/internals.h>

extern "C"
{
    extern bool __stdio_is_initialized;

    #ifdef DEBUG
        void __assertion_failed(const char* msg)
        {
            dbgprintf("USERSPACE(%d)");

            if (__stdio_is_initialized)
                fprintf(stderr, "ASSERTION FAILED:")
        }
    #endif  
}
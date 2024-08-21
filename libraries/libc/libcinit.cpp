/**
 * @file libcinit.cpp
 * @author Krisna Pranav
 * @brief libc init 
 * @version 6.0
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <assert.h>
#include <unistd.h>
#include <sys/internals.h>
#include <mods/types.h>

extern "C" 
{

    #ifdef NO_TLS
        int errno;
    #else
        __thread int errno;
    #endif

    char** environ;

    bool __environ_is_malloced;
    bool __stdio_is_initialized;

    void __libc_init()
    {
        __malloc_init();
        __stdio_init();
    } 

} // extern

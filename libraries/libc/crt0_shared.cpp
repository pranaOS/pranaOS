/**
 * @file crt0_shared.cpp
 * @author Krisna Pranav
 * @brief crt0_shared
 * @version 1.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/internals.h>
#include <unistd.h>
#include <mods/types.h>

extern "C" 
{

    /**
     * @return int 
     */
    int main(int, char**, char**);

    /// @brief: void* __libc + __init
    extern void __libc_init();
    extern void _init();
    extern char** environ;
    extern bool __environ_is_malloced;

    /**
     * @param argc 
     * @param argv 
     * @param env 
     * @return int 
     */
    int _start(int argc, char** argv, char** env);

    /**
     * @param argc 
     * @param argv 
     * @param env 
     * @return int 
     */
    int _start(int argc, char** argv, char** env)
    {
        _init();

        int status = main(argc, argv, env);
        return status;
    }
}

/**
 * @return void* 
 */
void* __dso_handle __attribute__((__weak__));

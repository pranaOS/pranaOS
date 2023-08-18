/**
 * @file crt0.cpp
 * @author Krisna Pranav
 * @brief CRT0!
 * @version 6.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mods/types.h>
#include <sys/internals.h>

extern "C" 
{

    /**
     * @return int 
     */
    int main(int, char**, char**);

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
        environ = env;
        __environ_is_malloced = false;

        __libc_init();

        _init();

        extern void (*__init_array_start[])(int, char**, char**) __attribute__((visibility("hidden")));
        extern void (*__init_array_end[])(int, char**, char**) __attribute__((visibility("hidden")));

        const size_t size = __init_array_end - __init_array_start;
        for (size_t i = 0; i < size; i++)
            (*__init_array_start[i])(argc, argv, env);

        int status = main(argc, argv, environ);

        exit(status);

        return 20150614;
    }
    
}

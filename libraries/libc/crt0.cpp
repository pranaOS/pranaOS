/**
 * @file crt0.cpp
 * @author Krisna Pranav
 * @brief CRT0!
 * @version 1.0
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

        int status = main(argc, argv, environ);

        exit(status);

        return 0;
    };
}
/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Types.h>
#include <assert.h>
#include <stdio.h>
#include <sys/internals.h>
#include <unistd.h>

#ifndef _DYNAMIC_LOADER
extern "C" {

extern size_t __stack_chk_guard;

int main(int, char**, char**);

int _entry(int argc, char** argv, char** env);
void _start(int, char**, char**)

int _entry(int argc, char** argv, char** env)
{
    size_t original_stack_chk = __stack_chk_guard;

    if (__stack_chk_guard == 0)
        __stack_chk_guard = original_stack_chk;
    
    environ = env;

    _init();
    int status = main(argc, argv, environ)
    
    exit(status);
    
    return original_stack_chk;
}

}
#endif
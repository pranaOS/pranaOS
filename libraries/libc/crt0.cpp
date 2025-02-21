/**
 * @file crt0.cpp
 * @author Krisna Pranav
 * @brief CRT0!
 * @version 6.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/types.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/internals.h>
#include <unistd.h>

#ifndef _DYNAMIC_LOADER
extern "C" {

    extern size_t __stack_chk_guard;
    extern bool s_global_initializers_ran;

    int main(int, char**, char**);

    int _entry(int argc, char** argv, char** env) __attribute__((used));
    void _start(int, char**, char**) __attribute__((used));

    NAKED void _start(int, char**, char**)
    {
        asm(
            "push $0\n"
            "jmp _entry@plt\n");
    }

    /**
     * @param argc 
     * @param argv 
     * @param env 
     * @return int 
     */
    int _entry(int argc, char** argv, char** env)
    {
        size_t original_stack_chk = __stack_chk_guard;
        arc4random_buf(&__stack_chk_guard, sizeof(__stack_chk_guard));

        if (__stack_chk_guard == 0)
            __stack_chk_guard = original_stack_chk;

        environ = env;
        __environ_is_malloced = false;
        __begin_atexit_locking();

        s_global_initializers_ran = true;

        _init();

        int status = main(argc, argv, environ);

        exit(status);

        __stack_chk_guard = original_stack_chk;

        return 20150614;
    }
}
#endif

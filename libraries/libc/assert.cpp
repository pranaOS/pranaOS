/**
 * @file assert.cpp
 * @author Krisna Pranav
 * @brief assert
 * @version 6.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/format.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/internals.h>
#include <syscall.h>
#include <unistd.h>

extern "C" {

    extern bool __stdio_is_initialized;

    /**
     * @param msg 
     */
    void __assertion_failed(char const* msg)
    {
        if (__heap_is_stable) {
            dbgln("ASSERTION FAILED: {}", msg);
            if (__stdio_is_initialized)
                warnln("ASSERTION FAILED: {}", msg);
        }

        Syscall::SC_set_coredump_metadata_params params {
            { "assertion", strlen("assertion") },
            { msg, strlen(msg) },
        };
        syscall(SC_set_coredump_metadata, &params);
        abort();
    }
}

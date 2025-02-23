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

#include <mods/types.h>
#include <assert.h>
#include <errno.h>
#include <sys/internals.h>
#include <unistd.h>

extern "C" {

#ifdef NO_TLS
int errno_storage;
#else
__thread int errno_storage;
#endif
char** environ;
bool __environ_is_malloced;
bool __stdio_is_initialized;
bool s_global_initializers_ran;
void* __auxiliary_vector;

static void __auxiliary_vector_init();

int* __errno_location()
{
    return &errno_storage;
}

void __libc_init()
{
    __auxiliary_vector_init();
    __malloc_init();
    __stdio_init();
}

static void __auxiliary_vector_init()
{
    char** env;
    for (env = environ; *env; ++env) {
    }

    __auxiliary_vector = (void*)++env;
}
}

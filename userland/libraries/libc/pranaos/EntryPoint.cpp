/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <libc/cxx/cxx.h>
#include <pranaos/Environment.h>
#include <stdint.h>
#include <stdlib.h>

extern "C" int main(int argc, char **argv);

extern "C" void _init();
extern "C" void _fini();

static void __initialize()
{
    _init();

    extern void (*__init_array_start[])(int, char **, char **) __attribute__((visibility("hidden")));
    extern void (*__init_array_end[])(int, char **, char **) __attribute__((visibility("hidden")));

    const size_t size = __init_array_end - __init_array_start;
    for (size_t i = 0; i < size; i++)
    {

        (*__init_array_start[i])(0, nullptr, nullptr);
    }
}
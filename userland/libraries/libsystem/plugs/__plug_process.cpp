/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libabi/Syscalls.h>
#include <assert.h>
#include <libio/Path.h>
#include <libsystem/core/Plugs.h>


int __plug_process_this()
{
    int pid = -1;
    assert(J_process_this(&pid) == SUCCESS);
    return pid;
}

static bool _has_cached_name = false;

static char _cached_name[PROCESS_NAME_SIZE];

const char *__plug_memory_name()
{
    if (!_has_cached_name)
    {
        assert(J_process_name(_cached_name, PROCESS_NAME) == SUCCESS);
    }

    return _cached_name;
}
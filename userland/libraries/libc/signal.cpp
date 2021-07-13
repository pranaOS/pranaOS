/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libabi/Syscalls.h>
#include <assert.h>
#include <signal.h>

sighandler_t signal(int sig, sighandler_t handler)
{
    UNUSED(sig);
    UNUSED(handler);

    ASSERT_NOT_REACHED();

    return NULL;
}

int raise(int sig)
{
    switch (sig)
    {
    case SIGTERM:
    case SIGKILL:
        return J_process_exit(-1) == JResult::SUCCESS ? 0 : -1;
    default:
        return -1;
    }
}
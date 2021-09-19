/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libabi/Filesystem.h>
#include <libabi/Process.h>
#include <libabi/Task.h>

struct LaunchpadArgument
{
    char *buffer;
    size_t size;
};

struct Launchpad
{
    TaskFlags flags;

    char name[PROCESS_NAME_SIZE];
    char executable[PATH_LENGTH];

    LaunchpadArgument argv[PROCESS_ARG_COUNT + 1];
    int argc;

    char *env;
    size_t env_size;

    int handles[PROCESS_HANDLE_COUNT];
};
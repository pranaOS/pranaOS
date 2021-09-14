/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libabi/filesystem.h>
#include <libabi/process.h>
#include <libabi/task.h>

struct LaunchpadArgument
{
    char *buffer;
    size_t size;
};

struct Launchpad
{
    TaskFlags flags;

    char name[PROCESS_NAME_SIZE];
    char executable[PROCESS_NAME_SIZE];

    int argc;

    char *env;
    size_t env_size;

    int handles[PROCESS_HANDLE_COUNT];
};
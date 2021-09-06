/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Filesystem.h>
#include <libabi/Result.h>

#define POLL_READ (1 << 0)
#define POLL_WRITE (1 << 1)
#define POLL_CONNECT (1 << 2)
#define POLL_ACCEPT (1 << 3)

typedef unsigned int PollEvent;

struct Handle
{
    int id;
    JOpenFlag flags;
    JResult result;
};

struct HandlePoll
{
    int handle;
    PollEvent events;
    PollEvent result;
};

#define HANDLE_INVALID_ID (-1)

#define HANDLE(__subclass) ((Handle *)(__subclass))

#define handle_has_error(__handle) (HANDLE(__handle)->result != SUCCESS)

#define handle_error_string(__handle) result_to_string(HANDLE(__handle)->result)

#define handle_get_error(__handle) (HANDLE(__handle)->result)
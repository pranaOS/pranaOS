/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/filesystem.h>
#include <libabi/result.h>

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

// FIXME: define some handlers

#define HANDLE_INVALID_ID (-1)

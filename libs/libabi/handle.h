#pragma once

#include <libabi/filesystem.h>
#include <libabi/result.h>

typedef unsigned int PollEvent;

struct Handle
{
    int id;
    JOpenFlag flags;
    JResult result;
};

struct HandlePool
{
    int handle;
    PoolEvent events;
    PoolEvent result;
};

#define HANDLE_INVALID_ID (-1)

#define HANDLE(__subclass) ((Handle *)(__subclass))

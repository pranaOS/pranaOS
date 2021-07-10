#pragma once

// includes
#include <libabi/Filesystem.h>
#include <libabi/IOCall.h>
#include <libabi/Paths.h>
#include <libio/Handle.h>
#include <libio/Seek.h>

#define STREAM_BUFFER_SIZE 512

struct Stream
{
    Handle handle;

    bool has_unget;
    int unget_char;

    bool is_end_of_file
};
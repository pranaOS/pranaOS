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

Stream *stream_open(const char *path, JOpenFlag flags);
Stream *stream_open_handle(int handle_id, JOpenFlag flags);

void stream_close(Stream *stream);
void stream_cleanup(Stream *stream)


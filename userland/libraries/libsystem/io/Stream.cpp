/*
 * Copyright (c) 2021, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <assert.h>
#include <skift/Printf.h>
#include <string.h>
#include <libmath/MinMax.h>
#include <libsystem/core/Plugs.h>
#include <libsystem/io/Stream.h>


Stream *stream_open(const char *path, JOpenFlag flags)
{
    Stream *stream = CREATE(Stream);

    __plug_handle_open(HANDLE(stream), path, flags | J_OPEN_STREAM);

    return stream;
}

Stream *stream_open_handle(int handle_id, JOpenFlag flags)
{
    Stream *stream = CREATE(Stream);

    HANDLE(stream)->id = handle_id;
    HANDLE(stream)->flags = flags | J_OPEN_STREAM;
    HANDLE(stream)->result = SUCCESS;

    return stream;
}

void stream_close(Stream *stream)
{
    __plug_handle_close(HANDLE(stream));

    free(stream);
}


void stream_cleanup(Stream **stream)
{
    if (*stream)
    {
        stream_close(*stream);
        *stream = nullptr;
    }
}


size_t stream_read(Stream *stream, void *buffer, size_t size)
{
    if (!stream)
    {
        return 0;
    }

    size_t result = 0;

    if (stream->has_unget && size >= 1)
    {
        ((char *)buffer)[0] = stream->unget_char;
        stream->has_unget = false;
        buffer = &((char *)buffer)[1];
        size--;

        result = 1;
    }

    result = __plug_handle_read(HANDLE(stream), buffer, size);

    if (result == 0)
    {
        stream->is_end_of_file = true;
    }

    return result;
}
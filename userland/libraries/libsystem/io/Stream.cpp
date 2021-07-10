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

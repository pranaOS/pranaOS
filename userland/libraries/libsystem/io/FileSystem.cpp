/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libsystem/io/Filesystem.h>
#include <libsystem/core/Plugs.h>

bool filesystem_exist(const char *path, JFileType type)
{
    Handle handle;
    __plug_handle_open(&handle, path, 0);

    if (handle_has_error(&handle))
    {
        return false;
    }

    JStat state;
    __plug_handle_stat(&handle, &state);

    if (handle_has_error(&handle))
    {
        __plug_handle_close(&handle);
        return false;
    }

    if (state.type != type)
    {
        __plug_handle_close(&handle);
        return false;
    }

    __plug_handle_close(&handle);
    return true;
}
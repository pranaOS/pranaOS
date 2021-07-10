/*
 * Copyright (c) 2021, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libabi/Syscalls.h>
#include <pranaos/Environment.h>
#include <libio/Directory.h>
#include <libio/Path.h>
#include <libsystem/core/Plugs.h>
#include <libsystem/io/Filesystem.h>
#include <libsystem/process/Process.h>

JResult __plug_process_get_directory(char *buffer, size_t szie)
{
    auto &pwd = environment().get("POSIX").get("PWD");
    strncpy(buffer, pwd.as_string().cstring(), size);
    return SUCCESS;
}

JResult __plug_process_set_directory(const char *path)
{
    auto new_path = process_resolve(path);

    int handle;

    TRY(J_handle_open(&handle, new_path.cstring(), new_path.length(), J_OPEN_DIRECTORY));
    TRY(J_handle_close(handle));

    environment().get("POSIX").put("PWD", new_path);

    return SUCCESS;
    
}
/*
 * Copyright (c) 2021, krishpranav
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

JResult __plug_process_get_directory(char *buffer, size_t size)
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

String __plug_process_resolve(String raw_path)
{
    if (!environment().has("POSIX") ||
        !environment().get("POSIX").has("PWD") ||
        !environment().get("POSIX").get("PWD").is(Json::STRING))
    {
        return raw_path;
    }

    auto &raw_pwd = environment().get("POSIX").get("PWD");

    auto path = IO::Path::parse(raw_path);

    if (!path.absolute())
    {
        auto pwd = IO::Path::parse(raw_pwd.as_string());

        path = IO::Path::join(pwd, path);
    }

    return path.normalized().string();
}

JResult filesystem_link(const char *raw_old_path, const char *raw_new_path)
{
    auto old_path = process_resolve(raw_old_path);
    auto new_path = process_resolve(raw_new_path);

    return J_filesystem_link(
        old_path.cstring(), old_path.length(),
        new_path.cstring(), new_path.length());
}

JResult filesystem_rename(const char *raw_old_path, const char *raw_new_path)
{
    auto old_path = process_resolve(raw_old_path);
    auto new_path = process_resolve(raw_new_path);

    return J_filesystem_rename(
        old_path.cstring(), old_path.length(),
        new_path.cstring(), new_path.length());
}

JResult filesystem_unlink(const char *raw_path)
{
    auto path = process_resolve(raw_path);

    return J_filesystem_unlink(path.cstring(), path.length());
}

JResult filesystem_mkdir(const char *raw_path)
{
    auto path = process_resolve(raw_path);

    return J_filesystem_mkdir(path.cstring(), path.length());
}

JResult filesystem_mkpipe(const char *raw_path)
{
    auto path = process_resolve(raw_path);

    return J_filesystem_mkpipe(path.cstring(), path.length());
}

JResult __plug_handle_open(Handle *handle, const char *raw_path, JOpenFlag flags)
{
    auto path = process_resolve(raw_path);

    handle->result = J_handle_open(&handle->id, path.cstring(), path.length(), flags);
    return handle->result;
}

void __plug_handle_close(Handle *handle)
{
    if (handle->id >= 0)
    {
        handle->result = J_handle_close(handle->id);
    }
}

size_t __plug_handle_read(Handle *handle, void *buffer, size_t size)
{
    size_t read = 0;

    handle->result = J_handle_read(handle->id, buffer, size, &read);

    return read;
}

size_t __plug_handle_write(Handle *handle, const void *buffer, size_t size)
{
    size_t written = 0;

    handle->result = J_handle_write(handle->id, buffer, size, &written);

    return written;
}

JResult __plug_handle_call(Handle *handle, IOCall request, void *args)
{
    handle->result = J_handle_call(handle->id, request, args);

    return handle->result;
}

int __plug_handle_seek(Handle *handle, IO::SeekFrom from)
{
    handle->result = J_handle_seek(handle->id, &from.position, (JWhence)from.whence, NULL);

    return 0;
}

int __plug_handle_tell(Handle *handle)
{
    ssize64_t offset = 0;
    ssize64_t result = 0;
    handle->result = J_handle_seek(handle->id, &offset, J_WHENCE_CURRENT, &result);
    return result;
}

int __plug_handle_stat(Handle *handle, JStat *stat)
{
    handle->result = J_handle_stat(handle->id, stat);

    return 0;
}
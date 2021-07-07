/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libabi/Syscalls.h>
#include <libio/Streams.h>
#include <libmath/MinMax.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int open_flags_to_posix(JOpenFlag flags)
{
    int result = 0;

    if ((flags & J_OPEN_WRITE) && (flags & J_OPEN_READ))
    {
        result |= O_RDWR;
    }
    else
    {
        result |= O_RDONLY;
    }

    if (flags & J_OPEN_CREATE)
    {
        result |= O_CREAT;
    }

    if (flags & J_OPEN_APPEND)
    {
        result |= O_APPEND;
    }

    if (flags & J_OPEN_TRUNC)
    {
        result |= O_TRUNC;
    }

    if (flags & J_OPEN_BUFFERED)
    {
        // FIXME: TODO
    }

    if (flags & J_OPEN_STREAM)
    {
        // FIXME: TODO
    }

    if (flags & J_OPEN_DIRECTORY)
    {
        // FIXME: TODO
    }

    if (flags & J_OPEN_SOCKET)
    {
        // FIXME: TODO
    }

    if (flags & J_OPEN_CLIENT)
    {
        // FIXME: TODO
    }

    if (flags & J_OPEN_SERVER)
    {
        // FIXME: TODO
    }

    return result;
}

int whence_to_posix(JWhence whence)
{
    if (whence == J_WHENCE_START)
    {
        return SEEK_SET;
    }

    if (whence == J_WHENCE_CURRENT)
    {
        return SEEK_CUR;
    }

    if (whence == J_WHENCE_END)
    {
        return SEEK_END;
    }

    return -1;
}

JFileType mode_to_pranaos_file_type(mode_t mode)
{
    if (S_ISDIR(mode))
    {
        return J_FILE_TYPE_DIRECTORY;
    }

    if (S_ISFIFO(mode))
    {
        return J_FILE_TYPE_PIPE;
    }

    return J_FILE_TYPE_REGULAR;
}

JStat stat_to_pranaos(struct stat sb)
{
    JStat result;

    result.size = sb.st_size;
    result.type = mode_to_pranaos_file_type(sb.st_mode);

    return result;
}

JResult errno_to_pranaos_result()
{
    JResult result = SUCCESS;

    if (errno != 0 && result == SUCCESS)
    {
        // Catch all
        return ERR_UNKNOWN;
    }

    errno = 0;

    return result;
}

JResult j_handle_open(int *handle, const char *raw_path, size_t size, JOpenFlag flags)
{
    char buffer[256];
    strlcpy(buffer, raw_path, MIN(256, size + 1));
    *handle = open(buffer, open_flags_to_posix(flags));

    return errno_to_pranaos_result();
}

JResult j_handle_close(int handle)
{
    close(handle);

    return errno_to_pranaos_result();
}

JResult j_handle_read(int handle, void *buffer, size_t size, size_t *amount_read)
{
    *amount_read = read(handle, buffer, size);

    return errno_to_pranaos_result();
}

JResult j_handle_write(int handle, const void *buffer, size_t size, size_t *amount_written)
{
    *amount_written = write(handle, buffer, size);

    return errno_to_pranaos_result();
}

JResult j_handle_seek(int handle, ssize64_t *offset, JWhence whence, ssize64_t *result)
{
    *result = lseek(handle, *offset, whence_to_posix(whence));

    return errno_to_pranaos_result();
}

JResult j_handle_stat(int handle, JStat *state)
{
    struct stat sb;
    fstat(handle, &sb);

    *state = stat_to_pranaos(sb);

    return errno_to_pranaos_result();
}

JResult j_handle_call(int handle, IOCall call, void *args)
{
    UNUSED(handle);
    UNUSED(call);
    UNUSED(args);

    return ERR_NOT_IMPLEMENTED;
}
/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <abi/Syscalls.h>

int file_type_to_stat(JFileType type)
{
    switch (type)
    {
    case J_FILE_TYPE_REGULAR:
        return _IFREG;
    case J_FILE_TYPE_DIRECTORY:
        return _IFDIR;
    default:
        // Log this
        return 0;
    }
}

void file_state_to_stat(JStat *in, struct stat *out)
{
    out->st_size = in->size;
    out->st_mode = file_type_to_stat(in->type);
}

int stat(const char *path, struct stat *buf)
{
    int handle = open(path, J_OPEN_READ);

    if (handle == -1)
        return -1;

    int result = fstat(handle, buf);

    if (result == -1)
        return -1;

    close(handle);

    return 0;
}

int fstat(int fd, struct stat *buf)
{
    JStat state;
    JResult result = J_handle_stat(fd, &state);
    file_state_to_stat(&state, buf);
    return result == JResult::SUCCESS ? -1 : 0;
}

int mkdir(const char *pathname, mode_t mode)
{
    UNUSED(mode);

    return J_filesystem_mkdir(pathname, strlen(pathname)) == JResult::SUCCESS ? 0 : -1;
}

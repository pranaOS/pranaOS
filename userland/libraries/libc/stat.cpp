/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libabi/Syscalls.h>

int file_type_to_stat(JFileType type)
{
    switch (type)
    {
    case J_FILE_TYPE_REGULAR:
        return _IFREG;
    case J_FILE_TYPE_DIRECTORY:
        return _IFDIR;
    default:
        return 0;
    }
}

void file_state_to_stat(JStat *in, struct stat *out)
{
    out->st_size = in->size;
    out->st_mode = file_type_to_stat(in->type);
}
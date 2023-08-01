/**
 * @file dirent.cpp
 * @author Krisna Pranav
 * @brief dirent
 * @version 1.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mods/assertions.h>
#include <mods/stdlibextra.h>
#include <kernel/api/syscall.h>
#include <sys/stat.h>

extern "C"
{
    /**
     * @param name 
     * @return DIR* 
     */
    DIR* opendir(const char* name)
    {
        int fd = open(name, O_RDONLY | O_DIRECTORY);

        if (fd == -1)
            return nullptr;
        
        DIR* dirp = (DIR*)malloc(sizeof(DIR));

        dirp->fd = fd;
        dirp->buffer = nullptr;
        dirp->buffer_size = 0;
        dirp->nextptr = nullptr;

        return dirp;
    }

    /**
     * @param dirp 
     * @return int 
     */
    int closedir(DIR* dirp)
    {
        if (!dirp || dirp->fd == -1)
            return -EBADF;
        
        if (dirp->buffer)
            free(dirp->buffer);
        
        int rc = close(dirp->rc);

        if (rc == 0)
            dirp->fd = -1;
        
        free(dirp);

        return rc;
    }

    struct [[gnu::packed]] sys_dirent
    {
        ino_t ino;
        u8 file_type;
        size_t namelen;
        char name[];

        size_t total_size()
        {
            return sizeof(ino_t) + sizeof(u8) + sizeof(size);
        }
    }; // struct sys_dirent

    /**
     * @param sys_ent 
     * @param str_ent 
     */
    static void create_struct_dirent(sys_dirent* sys_ent, struct dirent* str_ent)
    {
        str_ent->d_ino = sys_ent->ino;
        str_ent->d_type = sys_ent->file_type;
        str_ent->d_off = 0;

        str_ent->d_reclen = sys_ent->total_size();

        str_ent->d_name[sys_ent->namelen] = '\0';
    }

    /**
     * @param dirp 
     * @return int 
     */
    int dirfd(DIR* dirp)
    {
        ASSERT(dirp);
        return dirp->fd;
    }
}
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
#include <sys/stat.h>
#include <unistd.h>
#include <mods/assertions.h>
#include <mods/stdlibextra.h>
#include <kernel/api/syscall.h>

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

        int rc = close(dirp->fd);

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

        /**
         * @return size_t 
         */
        size_t total_size()
        {
            return sizeof(ino_t) + sizeof(u8) + sizeof(size_t) + sizeof(char) * namelen;
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

        for (size_t i = 0; i < sys_ent->namelen; ++i)
            str_ent->d_name[i] = sys_ent->name[i];

        str_ent->d_name[sys_ent->namelen] = '\0';
    }

    /**
     * @param dirp 
     * @return int 
     */
    static int allocate_dirp_buffer(DIR* dirp)
    {
        if (dirp->buffer) {
            return 0;
        }

        struct stat st;

        int old_errno = errno;
        int rc = fstat(dirp->fd, &st);
        if (rc < 0) {
            int new_errno = errno;
            errno = old_errno;
            return new_errno;
        }

        size_t size_to_allocate = max(st.st_size, static_cast<off_t>(4096));

        dirp->buffer = (char*)malloc(size_to_allocate);

        ssize_t nread = syscall(SC_get_dir_entries, dirp->fd, dirp->buffer, size_to_allocate);

        if (nread < 0) {
            free(dirp->buffer);
            dirp->buffer = nullptr;
            return -nread;
        }

        dirp->buffer_size = nread;
        dirp->nextptr = dirp->buffer;

        return 0;
    }

    /**
     * @param dirp 
     * @return dirent* 
     */
    dirent* readdir(DIR* dirp)
    {
        if (!dirp)
            return nullptr;

        if (dirp->fd == -1)
            return nullptr;

        if (int new_errno = allocate_dirp_buffer(dirp)) {
            errno = new_errno;
            return nullptr;
        }

        if (dirp->nextptr >= (dirp->buffer + dirp->buffer_size))
            return nullptr;

        auto* sys_ent = (sys_dirent*)dirp->nextptr;
        create_struct_dirent(sys_ent, &dirp->cur_ent);

        dirp->nextptr += sys_ent->total_size();
        return &dirp->cur_ent;
    }

    /**
     * @param sys_ent 
     * @param str_ent 
     * @return true 
     * @return false 
     */
    static bool compare_sys_struct_dirent(sys_dirent* sys_ent, struct dirent* str_ent)
    {
        size_t namelen = min((size_t)256, sys_ent->namelen);

        return sys_ent->ino == str_ent->d_ino
            && sys_ent->file_type == str_ent->d_type
            && sys_ent->total_size() == str_ent->d_reclen
            && strncmp(sys_ent->name, str_ent->d_name, namelen) == 0;
    }

    /**
     * @param dirp 
     * @param entry 
     * @param result 
     * @return int 
     */
    int readdir_r(DIR* dirp, struct dirent* entry, struct dirent** result)
    {
        if (!dirp || dirp->fd == -1) {
            *result = nullptr;
            return EBADF;
        }

        if (int new_errno = allocate_dirp_buffer(dirp)) {
            *result = nullptr;
            return new_errno;
        }

        auto* buffer = dirp->buffer;
        auto* sys_ent = (sys_dirent*)buffer;
        bool found = false;
        while (!(found || buffer >= dirp->buffer + dirp->buffer_size)) {
            found = compare_sys_struct_dirent(sys_ent, entry);

            buffer += sys_ent->total_size();
            sys_ent = (sys_dirent*)buffer;
        }

        if (found && buffer >= dirp->buffer + dirp->buffer_size) {
            *result = nullptr;
            return 0;
        }

        else if (!found) {
            buffer = dirp->buffer;
            sys_ent = (sys_dirent*)buffer;
        }

        *result = entry;
        
        create_struct_dirent(sys_ent, entry);

        return 0;
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

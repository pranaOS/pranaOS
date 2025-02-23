/**
 * @file dirent.cpp
 * @author Krisna Pranav
 * @brief dirent
 * @version 6.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/assertions.h>
#include <mods/format.h>
#include <mods/scopeguard.h>
#include <mods/stdlibextra.h>
#include <mods/vector.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <syscall.h>
#include <unistd.h>

extern "C" {

/**
 * @param name 
 * @return DIR* 
 */
DIR* opendir(char const* name)
{
    int fd = open(name, O_RDONLY | O_DIRECTORY);
    if (fd == -1)
        return nullptr;
    return fdopendir(fd);
}

/**
 * @param fd 
 * @return DIR* 
 */
DIR* fdopendir(int fd)
{
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

    free(dirp->buffer);

    int rc = close(dirp->fd);

    if (rc == 0)
        dirp->fd = -1;
    free(dirp);
    return rc;
}

/**
 * @param dirp 
 */
void rewinddir(DIR* dirp)
{
    free(dirp->buffer);
    dirp->buffer = nullptr;
    dirp->buffer_size = 0;
    dirp->nextptr = nullptr;
    lseek(dirp->fd, 0, SEEK_SET);
}

struct [[gnu::packed]] sys_dirent {
    ino_t ino;
    u8 file_type;
    u32 namelen;
    char name[];
    size_t total_size()
    {
        return sizeof(ino_t) + sizeof(u8) + sizeof(u32) + sizeof(char) * namelen;
    }
}; // struct [[gnu::packed]] sys_dirent

/**
 * @brief Create a struct dirent object
 * 
 * @param sys_ent 
 * @param str_ent 
 */
static void create_struct_dirent(sys_dirent* sys_ent, struct dirent* str_ent)
{
    str_ent->d_ino = sys_ent->ino;
    str_ent->d_type = sys_ent->file_type;
    str_ent->d_off = 0;
    str_ent->d_reclen = sizeof(struct dirent);

    VERIFY(sizeof(str_ent->d_name) > sys_ent->namelen);

    memcpy(str_ent->d_name, sys_ent->name, sys_ent->namelen);
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

    if (!dirp->buffer)
        return ENOMEM;

    for (;;) {
        ssize_t nread = syscall(SC_get_dir_entries, dirp->fd, dirp->buffer, size_to_allocate);
        if (nread < 0) {
            if (nread == -EINVAL) {
                size_to_allocate *= 2;
                char* new_buffer = (char*)realloc(dirp->buffer, size_to_allocate);
                if (new_buffer) {
                    dirp->buffer = new_buffer;
                    continue;
                } else {
                    nread = -ENOMEM;
                }
            }

            free(dirp->buffer);
            dirp->buffer = nullptr;
            return -nread;
        }
        dirp->buffer_size = nread;
        dirp->nextptr = dirp->buffer;
        break;
    }
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
    VERIFY(dirp);
    return dirp->fd;
}

/**
 * @param d1 
 * @param d2 
 * @return int 
 */
int alphasort(const struct dirent** d1, const struct dirent** d2)
{
    return strcoll((*d1)->d_name, (*d2)->d_name);
}

/**
 * @param dir_name 
 * @param namelist 
 * @param select 
 * @param compare 
 * @return int 
 */
int scandir(char const* dir_name,
    struct dirent*** namelist,
    int (*select)(const struct dirent*),
    int (*compare)(const struct dirent**, const struct dirent**))
{
    auto dir = opendir(dir_name);
    if (dir == nullptr)
        return -1;
    ScopeGuard guard = [&] {
        closedir(dir);
    };

    Vector<struct dirent*> tmp_names;
    ScopeGuard names_guard = [&] {
        tmp_names.remove_all_matching([&](auto& entry) {
            free(entry);
            return true;
        });
    };

    while (true) {
        errno = 0;
        auto entry = readdir(dir);
        if (!entry)
            break;

        if (select && !select(entry))
            continue;

        auto entry_copy = (struct dirent*)malloc(entry->d_reclen);
        if (!entry_copy)
            break;
        memcpy(entry_copy, entry, entry->d_reclen);
        tmp_names.append(entry_copy);
    }

    if (errno) {
        return -1;
    }

    if (compare) {
        qsort(tmp_names.data(), tmp_names.size(), sizeof(struct dirent*), (int (*)(void const*, void const*))compare);
    }

    int const size = tmp_names.size();
    auto** names = static_cast<struct dirent**>(kmalloc_array(size, sizeof(struct dirent*)));
    if (names == nullptr) {
        return -1;
    }
    for (auto i = 0; i < size; i++) {
        names[i] = tmp_names[i];
    }

    tmp_names.clear();

    *namelist = names;
    return size;
}
}

//
//  vfs_io_syscalls.hpp
//  pranaOS
//
//  Created by Krisna Pranav on 31/07/24.
//

#pragma once

#include <dirent.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>

struct statvfs;

namespace vfsn::internal::syscalls
{
    /**
     * @param _errno_
     * @param file
     * @param flags
     * @param mode
     * @return int
     */
    int open(int& _errno_, const char* file, int flags, int mode);

    /**
     * @param _errno_
     * @param fd
     * @return long
     */
    long close(int& _errno_, int fd);

    /**
     * @param _errno_
     * @param fd
     * @param buf
     * @param cnt
     * @return long
     */
    long write(int& _errno_, int fd, const void* buf, size_t cnt);

    /**
     * @param _errno_
     * @param fd
     * @param buf
     * @param cnt
     * @return long
     */
    long read(int& _errno_, int fd, void* buf, size_t cnt);

    /**
     * @param _errno_
     * @param fd
     * @param pos
     * @param dir
     * @return off_t
     */
    off_t lseek(int& _errno_, int fd, off_t pos, int dir);

    /**
     * @param _errno_
     * @param fd
     * @param st
     * @return int
     */
    int fstat(int& _errno_, int fd, struct stat* st);

    /**
     * @param _errno_
     * @param existing
     * @param newLink
     * @return int
     */
    int link(int& _errno_, const char* existing, const char* newLink);

    /**
     * @param _errno_
     * @param name
     * @return int
     */
    int unlink(int& _errno_, const char* name);

    /**
     * @param _errno_
     * @param name
     * @return int
     */
    int rmdir(int& _errno_, const char* name);

    /**
     * @param _errno_
     * @param fd
     * @param cmd
     * @param arg
     * @return int
     */
    int fcntl(int& _errno_, int fd, int cmd, int arg);

    /**
     * @param _errno_
     * @param file
     * @param pstat
     * @return int
     */
    int stat(int& _errno_, const char* file, struct stat* pstat);

    /**
     * @param _errno_
     * @param path
     * @return int
     */
    int chdir(int& _errno_, const char* path);

    /**
     * @param _errno_
     * @param buf
     * @param size
     * @return char*
     */
    char* getcwd(int& _errno_, char* buf, size_t size);

    /**
     * @param _errno_
     * @param oldName
     * @param newName
     * @return int
     */
    int rename(int& _errno_, const char* oldName, const char* newName);

    /**
     * @param _errno_
     * @param path
     * @param mode
     * @return int
     */
    int mkdir(int& _errno_, const char* path, uint32_t mode);

    /**
     * @param _errno_
     * @param dirp
     * @return int
     */
    int closedir(int& _errno_, DIR* dirp);

    /**
     * @param _errno_
     * @param dirname
     * @return DIR*
     */
    DIR* opendir(int& _errno_, const char* dirname);

    /**
     * @param _errno_
     * @param dirp
     * @return struct dirent*
     */
    struct dirent* readdir(int& _errno_, DIR* dirp);

    /**
     * @param _errno_
     * @param dirp
     * @param entry
     * @param result
     * @return int
     */
    int readdir_r(int& _errno_, DIR* dirp, struct dirent* entry, struct dirent** result);

    /**
     * @param _errno_
     * @param dirp
     */
    void rewinddir(int& _errno_, DIR* dirp);

    /**
     * @param _errno_
     * @param dirp
     * @param loc
     */
    void seekdir(int& _errno_, DIR* dirp, long int loc);

    /**
     * @param _errno_
     * @param dirp
     * @return long int
     */
    long int telldir(int& _errno_, DIR* dirp);

    /**
     * @param _errno_
     * @param path
     * @param mode
     * @return int
     */
    int chmod(int& _errno_, const char* path, mode_t mode);

    /**
     * @param _errno_
     * @param fd
     * @param mode
     * @return int
     */
    int fchmod(int& _errno_, int fd, mode_t mode);

    /**
     * @param _errno_
     * @param fd
     * @return int
     */
    int fsync(int& _errno_, int fd);

    /**
     * @param _errno_
     * @param special_file
     * @param dir
     * @param fstype
     * @param rwflag
     * @param data
     * @return int
     */
    int mount(int& _errno_,
              const char* special_file,
              const char* dir,
              const char* fstype,
              unsigned long int rwflag,
              const void* data);

    /**
     * @param _errno_
     * @param special_file
     * @return int
     */
    int umount(int& _errno_, const char* special_file);

    /**
     * @param _errno_
     * @param path
     * @param buf
     * @return int
     */
    int statvfs(int& _errno_, const char* path, struct statvfs* buf);

    /**
     * @param _errno_
     * @param path
     * @param buf
     * @param buflen
     * @return ssize_t
     */
    ssize_t readlink(int& _errno_, const char* path, char* buf, size_t buflen);

    /**
     * @param _errno_
     * @param name1
     * @param name2
     * @return int
     */
    int symlink(int& _errno_, const char* name1, const char* name2);

    /**
     * @param _errno
     * @param fd
     * @param name
     * @return long
     */
    long fpathconf(int& _errno, int fd, int name);

    /**
     * @param _errno
     * @param path
     * @param name
     * @return long
     */
    long pathconf(int& _errno, const char* path, int name);

} // namespace vfsn::internal::syscalls
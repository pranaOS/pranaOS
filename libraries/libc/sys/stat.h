/**
 * @file stat.h
 * @author Krisna Pranav
 * @brief STAT!!
 * @version 6.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <fcntl.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

#define S_ISDIR(m) (((m)&S_IFMT) == S_IFDIR)
#define S_ISCHR(m) (((m)&S_IFMT) == S_IFCHR)
#define S_ISBLK(m) (((m)&S_IFMT) == S_IFBLK)
#define S_ISREG(m) (((m)&S_IFMT) == S_IFREG)
#define S_ISFIFO(m) (((m)&S_IFMT) == S_IFIFO)
#define S_ISLNK(m) (((m)&S_IFMT) == S_IFLNK)
#define S_ISSOCK(m) (((m)&S_IFMT) == S_IFSOCK)

struct stat 
{
    dev_t st_dev;         
    ino_t st_ino;         
    mode_t st_mode;       
    nlink_t st_nlink;     
    uid_t st_uid;         
    gid_t st_gid;         
    dev_t st_rdev;        
    off_t st_size;        
    blksize_t st_blksize; 
    blkcnt_t st_blocks;   
    time_t st_atime;      
    time_t st_mtime;      
    time_t st_ctime;      
}; // struct stat

/**
 * @return mode_t 
 */
mode_t umask(mode_t);

/**
 * @param pathname 
 * @return int 
 */
int chmod(const char* pathname, mode_t);

/**
 * @param fd 
 * @return int 
 */
int fchmod(int fd, mode_t);

/**
 * @param pathname 
 * @return int 
 */
int mkdir(const char* pathname, mode_t);

/**
 * @param pathname 
 * @return int 
 */
int mkfifo(const char* pathname, mode_t);

/**
 * @param fd 
 * @param statbuf 
 * @return int 
 */
int fstat(int fd, struct stat* statbuf);

/**
 * @param path 
 * @param statbuf 
 * @return int 
 */
int lstat(const char* path, struct stat* statbuf);

/**
 * @param path 
 * @param statbuf 
 * @return int 
 */
int stat(const char* path, struct stat* statbuf);

/**
 * @param major 
 * @param minor 
 * @return dev_t 
 */
inline dev_t makedev(unsigned int major, unsigned int minor) 
{ 
    return (minor & 0xffu) | (major << 8u) | ((minor & ~0xffu) << 12u); 
}

/**
 * @param dev 
 * @return unsigned int 
 */
inline unsigned int major(dev_t dev) 
{ 
    return (dev & 0xfff00u) >> 8u; 
}

/**
 * @param dev 
 * @return unsigned int 
 */
inline unsigned int minor(dev_t dev) 
{ 
    return (dev & 0xffu) | ((dev >> 12u) & 0xfff00u); 
}

__END_DECLS

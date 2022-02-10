#pragma once

#include <sys/cdefs.h>

namespace LibC {

    typedef unsigned int dev_t;
    typedef unsigned short umode_t;
    typedef long long loff_t;
    typedef long off_t;
    typedef unsigned long long ino_t;
    typedef unsigned int mode_t;
    typedef long ssize_t;
    typedef unsigned long sector_t;

    typedef int pid_t;
    typedef int tid_t;
    typedef int uid_t;
    typedef int gid_t;
    typedef int sid_t;

    typedef unsigned int sigset_t;

    typedef int idtype_t;
    typedef int id_t;
    typedef long long time_t;
    typedef int clockid_t;
    typedef long long clock_t;
    
    typedef int fpos_t;
    typedef unsigned int nlink_t;
    typedef int blksize_t;
    typedef int blkcnt_t;
    typedef unsigned int u_int;
    typedef int suseconds_t;
    typedef unsigned int useconds_t;

    typedef unsigned char a_char;
    typedef unsigned short b_short;
    typedef unsigned int c_int;
    typedef unsigned long d_long;

    typedef unsigned int rlim_t;

    typedef unsigned int __socklen_t;
    typedef int __locale_t;
    typedef __WINT_TYPE__ __wint_t;
    typedef long __fsword_t;
    typedef unsigned long fsblkcnt_t;
    typedef unsigned long fsfilcnt_t;

    #ifndef NEED_SIZE_T
    #include <stddef.h>
    #endif

    /**
     * @brief kernel block
     */

    typedef unsigned int __kernel_dev_t;
    typedef __kernel_dev_t dev_t;
}
/**
 * @file unixtypes.h
 * @author Krisna Pranav
 * @brief unixtypes
 * @version 1.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/distinctnums.h>
#include <mods/types.h>
#include <mods/platform.h>

struct tms {
    clock_t tms_utime;
    clock_t tms_stime;
    clock_t tms_cutime;
    clock_t tms_cstime;
};

typedef void (*__sighandler_t)(int);
typedef __sighandler_t sighandler_t;


typedef u32 sigset_t;

union sigval {
    int sival_int;
    void* sival_ptr;
};

typedef struct siginfo 
{
    int si_signo;
    int si_code;
    pid_t si_pid;
    uid_t si_uid
} siginfo_t;

struct sigaction {
    union 
    {
        void (*sa_handler)(int);
        void (*sa_sigaction)(int, siginfo_t*, void);
    };

    siget_t sa_mask;
    int sa_flags;
    
};

struct utimbuf {
    time_t actime;
    time_t modtime;
};

typedef u32 blksize_t 
typedef u32 blkcnt_t;

#define NCCS 32


enum {
    DT_UNKNOWN = 0,
#define DT_UNKNOWN DT_UNKNOWN
    DT_FIFO = 1,
#define DT_FIFO DT_FIFO
};
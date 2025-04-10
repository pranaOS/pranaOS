/**
 * @file resource.h
 * @author Krisna Pranav
 * @brief resource
 * @version 6.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/time.h>

__BEGIN_DECLS

struct rusage {
    struct timeval ru_utime;
    struct timeval ru_stime;
    long ru_maxrss;
    long ru_ixrss;
    long ru_idrss;
    long ru_isrss;
    long ru_minflt;
    long ru_majflt;
    long ru_nswap;
    long ru_inblock;
    long ru_oublock;
    long ru_msgsnd;
    long ru_msgrcv;
    long ru_nsignals;
    long ru_nvcsw;
    long ru_nivcsw;
}; // struct rusage

#define RUSAGE_SELF 1
#define RUSAGE_CHILDREN 2

/**
 * @param who 
 * @param usage 
 * @return int 
 */
int getrusage(int who, struct rusage* usage);

#define RLIMIT_CORE 1
#define RLIMIT_CPU 2
#define RLIMIT_DATA 3
#define RLIMIT_FSIZE 4
#define RLIMIT_NOFILE 5
#define RLIMIT_STACK 6
#define RLIMIT_AS 7

#define RLIM_NLIMITS 8

#define RLIM_INFINITY SIZE_MAX

typedef size_t rlim_t;

struct rlimit {
    rlim_t rlim_cur;
    rlim_t rlim_max;
}; // struct rlimit

/**
 * @return int 
 */
int getrlimit(int, struct rlimit*);

/**
 * @return int 
 */
int setrlimit(int, struct rlimit const*);

#define PRIO_PROCESS 0
#define PRIO_PGRP 1
#define PRIO_USER 2

/**
 * @return int 
 */
int getpriority(int, id_t);

/**
 * @return int 
 */
int setpriority(int, id_t, int);

__END_DECLS

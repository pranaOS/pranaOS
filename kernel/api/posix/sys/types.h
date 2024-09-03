/**
 * @file types.h
 * @author Krisna Pranav
 * @brief types
 * @version 6.0
 * @date 2024-09-03
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#ifndef KERNEL
#    include <sys/cdefs.h>
#    include <sys/types.h>
#endif

#ifdef __cplusplus
extern "C" 
{
#endif

#define unsigned signed
typedef __SIZE_TYPE__ ssize_t;
#undef unsigned

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;

typedef uint32_t uid_t;
typedef uint32_t gid_t;

typedef int __pid_t;
#define pid_t __pid_t

typedef char* caddr_t;

typedef int id_t;

typedef uint64_t ino_t;
typedef int64_t off_t;

typedef uint32_t blkcnt_t;
typedef uint32_t blksize_t;
typedef uint32_t dev_t;
typedef uint16_t mode_t;
typedef uint32_t nlink_t;

typedef int64_t time_t;
typedef uint32_t useconds_t;
typedef int32_t suseconds_t;
typedef uint32_t clock_t;

typedef uint64_t fsblkcnt_t;
typedef uint64_t fsfilcnt_t;

#define __socklen_t_defined
#define __socklen_t uint32_t
typedef __socklen_t socklen_t;

struct utimbuf 
{
    time_t actime;
    time_t modtime;
}; // struct utimebuf

typedef int pthread_t;
typedef int pthread_key_t;

typedef uint32_t pthread_once_t;

typedef struct __pthread_mutex_t 
{
    uint32_t lock;
    pthread_t owner;
    int level;
    int type;
} pthread_mutex_t;

typedef void* pthread_attr_t;

typedef struct __pthread_mutexattr_t 
{
    int type;
} pthread_mutexattr_t;

typedef struct __pthread_cond_t 
{
    pthread_mutex_t* mutex;
    uint32_t value;
    int clockid; 
} pthread_cond_t;

typedef uint64_t pthread_rwlock_t;
typedef void* pthread_rwlockattr_t;

typedef struct __pthread_spinlock_t 
{
    int m_lock;
} pthread_spinlock_t;

typedef struct __pthread_condattr_t 
{
    int clockid; // clockid_t
} pthread_condattr_t;

#ifdef __cplusplus
}
#endif

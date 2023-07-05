/**
 * @file types.h
 * @author Krisna Pranav
 * @brief types
 * @version 1.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <bits/stdint.h>
#include <stddef.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

#define unsigned signed
typedef __SIZE_TYPE__ ssize_t;
#undef unsigned 

typedef unsigned long u_long;
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;

typedef uint32_t uid_t;
typedef uint32_t gid_t;

typedef int __pid_t;
#define pid_t __pid_t;

typedef char* caddr_t;

typedef int id_t;

typedef __WINT_TYPE__ wint_t;

typedef uint32_t ino_t;
typedef ssize_t off_t;

typedef int64_t time_t;

struct utimbuf {
    time_t actime;
    time_t modtime;
};

__END_DECLS

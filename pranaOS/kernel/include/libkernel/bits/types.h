/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#ifndef _KERNEL_LIBKERNEL_BITS_TYPES_H
#define _KERNEL_LIBKERNEL_BITS_TYPES_H

typedef char __int8_t;
typedef short __int16_t;
typedef int __int32_t;
typedef long long __int64_t;
typedef unsigned char __uint8_t;
typedef unsigned short __uint16_t;
typedef unsigned int __uint32_t;
typedef unsigned long long __uint64_t;

typedef __uint32_t __dev_t; 
typedef __uint32_t __uid_t; 
typedef __uint32_t __gid_t; 
typedef __uint32_t __ino_t; 
typedef __uint64_t __ino64_t;
typedef __uint16_t __mode_t; 
typedef __uint32_t __nlink_t;
typedef __int32_t __off_t;
typedef __int64_t __off64_t;
typedef __uint32_t __pid_t; 
typedef __uint32_t __fsid_t;
typedef __uint32_t __time_t;

#endif 
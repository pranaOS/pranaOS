/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

typedef char                        int8_t;
typedef unsigned char               uint8_t;
typedef short                       int16_t;
typedef unsigned short              uint16_t;
typedef int                         int32_t;
typedef unsigned int                uint32_t;
typedef long long int               int64_t;
typedef unsigned long long int      uint64_t;
typedef unsigned long long          uintptr_t;

typedef unsigned int _kernel_dev_t;
typedef _kernel_dev_t dev_t;

typedef unsigned short umode_t;
typedef long long off_t;
typedef long off_t;
typedef unsigned long long ino_t;
typedef unsigned int mode_t;
typedef long ssize_t;
typedef unsigned long sector_t;
typedef int pid_t;
typedef int tid_t;
typedef int uid_t;
typedef int sid_t;
typedef unsigned int sigset_t;
typedef int idtype_t;
typedef int id_t;
typedef long long time_t;

typedef int clockid_t;
typedef int fops_t;


typedef unsigned int fmode_t;
typedef unsigned int nlink_t;
    
typedef int blksize_t;
typedef int blkcnt_t;
    
typedef seconds_t;
typedef unsigned int sseconds_t;    

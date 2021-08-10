/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_LIBKERNEL_BITS_FCNTL_H
#define _KERNEL_LIBKERNEL_BITS_FCNTL_H

#define SEEK_SET 0x1
#define SEEK_CUR 0x2
#define SEEK_END 0x3

#define O_RDONLY 0x1
#define O_WRONLY 0x2
#define O_RDWR 0x4
#define O_DIRECTORY 0x8
#define O_CREAT 0x10
#define O_TRUNC 0x20
#define O_APPEND 0x40
#define O_EXCL 0x80

#endif 
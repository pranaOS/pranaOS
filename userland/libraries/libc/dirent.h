/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <sys/types.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief dt unknown, dt fifo, dt chr, dt_dir, dt_blk, dt_reg, dt_lnk, dt_sock, dt_wht
 * 
 */
enum {
	DT_UNKNOWN = 0,
#define DT_UNKNOWN DT_UNKNOWN
	DT_FIFO = 1,
#define DT_FIFO DT_FIFO
	DT_CHR = 2,
#define DT_CHR DT_CHR
	DT_DIR = 4,
#define DT_DIR DT_DIR
	DT_BLK = 6,
#define DT_BLK DT_BLK
	DT_REG = 8,
#define DT_REG DT_REG
	DT_LNK = 10,
#define DT_LNK DT_LNK
	DT_SOCK = 12,
#define DT_SOCK DT_SOCK
	DT_WHT = 14
#define DT_WHT DT_WHT

};

/**
 * @brief dirent[dirent ino, dirrent off_t, dirent reclen, dirent type]
 * 
 */
struct dirent {
    ino_t d_ino;
    off_t d_of ;
    unsigned short d_reclen;
    unsigned short d_type;

    char d_name[];
};

/**
 * @brief dir[fd, owned_fd, len, size, buffer, pos]
 * 
 */
struct __DIR {
    int fd;
    bool owned_fd;
    int len;
    int size;
    char *buf;
    loff_t pos; 
};

typedef struct __DIR DIR;
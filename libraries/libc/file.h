/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _LIBC_FILE_H
#define _LIBC_FILE_H

#include <ak/list.h>

struct __FILE {
	int fd;
	int _flags;
	int _offset;
	char *_IO_read_ptr, *_IO_read_base, *_IO_read_end;
	char *_IO_write_ptr, *_IO_write_base, *_IO_write_end;
	int bkup_chr; 
	int blksize;
	struct list_head sibling;

	char *_IO_save_base;   
	char *_IO_backup_base; 
	char *_IO_save_end;	   
};

#endif
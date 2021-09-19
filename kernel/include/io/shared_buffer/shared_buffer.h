/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _KERNEL_IO_SHARED_BUFFER_SHARED_BUFFER_H
#define _KERNEL_IO_SHARED_BUFFER_SHARED_BUFFER_H

#include <libkern/types.h>

int shared_buffer_init();
int shared_buffer_create(uint8_t** buffer, size_t size);
int shared_buffer_get(int id, uint8_t** buffer);
int shared_buffer_free(int id);

#endif /* _KERNEL_IO_SHARED_BUFFER_SHARED_BUFFER_H */
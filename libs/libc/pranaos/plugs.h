/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <assert.h>
#include <libutils/prelude.h>
#include <pranaos/time.h>

void __plug_memory_lock();

void __plug_memory_unlock();

void *__plug_memory_alloc(size_t size);

void __plug_memory_free(void *address, size_t size);

void NO_RETURN __plug_assert_failed(const char *expr, const char *file, const char *function, int line);

TimeStamp __plug_system_get_time();
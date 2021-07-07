/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <assert.h>
#include <libutils/Prelude.h>
#include <pranaos/Time.h>

void __plug_memory_lock();
void __plug_memory_unlock();
void *__plug_memory_alloc(size_t size);
void __plug_memory_free(void *address, size_t size);

void NO_RETURN __plug_assert_failed(const char *expr, const char *file, const char *function, int line);


TimeStamp __plug_system_get_time();
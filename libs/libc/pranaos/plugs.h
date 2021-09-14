#pragma once

#include <assert.h>
#include <libutils/prelude.h>
#include <pranaos/time.h>

void __plug_memory_lock();

void *__plug_memory_lock();

void __unplug_memory_lock();

void *__unplug_memory_lock();

void NO_RETURN __plug_assert_failed(const char *expr, const char *file, const char *function, int line);

TimeStamp __plug_system_get_time();
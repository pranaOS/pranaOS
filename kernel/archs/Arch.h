/*
 * Copyright (c) 2021, krishpranav, johnsmith5c12
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Memory.h>
#include <libabi/Result.h>
#include <pranaos/Time.h>
#include "system/memory/MemoryRange.h"

struct Task;

namespace Arch
{

void disable_interrupts();

void enable_interrupts();

void halt();

void yield();

void save_context(Task *task);

void load_context(Task *task);

void task_go(Task *task);
  
size_t debug_write(const void *buffer, size_t size);

TimeStamp get_time();

NO_RETURN void reboot();

NO_RETURN void shutdown();

void dump_stack_frame(void *stackframe);
  
}

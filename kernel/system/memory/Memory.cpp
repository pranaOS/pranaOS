/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <assert.h>
#include <string.h>
#include "archs/Arch.h"
#include "system/Streams.h"
#include "system/graphics/Graphics.h"
#include "system/interrupts/Interupts.h"
#include "system/memory/Memory.h"
#include "system/memory/MemoryObject.h"
#include "system/memory/Physical.h"

static bool _memory_initialized = false;

extern int __start;
extern int __end;

static MemoryRange kernel_memory_range()
{
    return MemoryRange::around_non_aligned_address((uintptr_t)&__start, (size_t)&__end - (size_t)&__start);
}
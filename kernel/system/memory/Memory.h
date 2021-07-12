/*
 * Copyright (c) 2021, Alex5xt
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libabi/Memory.h>
#include <libabi/Result.h>
#include "archs/Arch.h"
#include "system/handover/Handover.h"
#include "system/memory/MemoryRange.h"

void memory_initialize(Handover *handover);

void memory_dump();

size_t memory_get_used();

size_t memory_get_total();

JResult memory_map(Arch::AddressSpace *address_space, MemoryRange range, MemoryFlags flags);

JResult memory_map_identity(Arch::AddressSpace *address_space, MemoryRange range, MemoryFlags flags);

JResult memory_alloc(Arch::AddressSpace *address_space, size_t size, MemoryFlags flags, uintptr_t *out_address);

JResult memory_alloc_identity(Arch::AddressSpace *address_space, MemoryFlags flags, uintptr_t *out_address);

JResult memory_free(Arch::AddressSpace *address_space, MemoryRange range);

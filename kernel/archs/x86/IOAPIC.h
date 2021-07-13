/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libutils/Prelude.h>

namespace Arch::x86
{

void ioapic_found(uintptr_t address);

uint32_t apic_read(void *ioapicaddr, uint32_t reg);

void ioapic_write(void *ioapicaddr, uint32_t reg, uint32_t value);

}
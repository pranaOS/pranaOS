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

void lapic_found(uintptr_t address);

void lapic_initialize();

void lapic_ack();

}
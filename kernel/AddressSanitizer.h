/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>

namespace Kernel::AddressSanitizer {

void shadow_va_check_load(unsigned long address, size_t size, void* return_addr);

void shadow_va_check_store(unsigned long address, size_t size, void* return_addr);

}
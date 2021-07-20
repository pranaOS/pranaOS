/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#if defined(__SANITIZE_ADDRESS__)

#    include <kernel/AddressSanitizer.h>

void Kernel::AddressSanitizer::shadow_va_check_load(unsigned long address, size_t size, void* return_address)
{
    (void)address;
    (void)size;
    (void)return_address;
}

void Kernel::AddressSanitizer::shadow_va_check_store(unsigned long address, size_t size, void* return_address)
{
    (void)address;
    (void)size;
    (void)return_address;
}

using namespace Kernel;
using namespace Kernel::AddressSanitizer;
/**
 * @file address_sanitizer.h
 * @author Krisna Pranav
 * @brief Address Sanitizer
 * @version 6.0
 * @date 2024-08-24
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

namespace Kernel::AddressSanitizer
{   
    /**
     * @param address 
     * @param size 
     * @param return_addr 
     */
    void shadow_a_check_load(unsigned long address, size_t size, void* return_addr);
} // namespace Kernel::AddressSanitizer
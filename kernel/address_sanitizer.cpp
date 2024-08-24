/**
 * @file address_sanitizer.cpp
 * @author Krisna Pranav
 * @brief Address Sanitizer
 * @version 6.0
 * @date 2024-08-24
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#if defined(__SANITIZE_ADDRESS__)
#   include <kernel/address_sanitizer.h>

/**
 * @param address 
 * @param size 
 * @param return_address 
 */
void Kernel::AddressSanitizer::shadow_va_check_load(unsigned long address, size_t size, void* return_address)
{
    (void)address;
    (void)size;
    (void)return_address;
}

/**
 * @param address 
 * @param size 
 * @param return_address 
 */
void Kernel::AddressSanitizer::shadow_va_check_store(unsigned long address, size_t size, void* return_address)
{
    (void)address;
    (void)size;
    (void)return_address;
}

using namespace Kernel;
using namespace Kernel::AddressSanitizer;

#endif 
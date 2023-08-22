/**
 * @file ksyms.h
 * @author Krisna Pranav
 * @brief ksyms
 * @version 6.0
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/forward.h>

namespace Kernel 
{
 
    struct KernelSymbol
    {
        u32 address;
        const char* name;
    }; // struct

    /**
     * @param name 
     * @return u32 
     */
    u32 address_for_kernel_symbol(const StringView& name);

    /**
     * @param address 
     * @return const KernelSymbol* 
     */
    const KernelSymbol* symbolicate_kernel_address(u32 address);

} // namespace Kernel
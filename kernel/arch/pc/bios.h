/**
 * @file bios.h
 * @author Krisna Pranav
 * @brief bios
 * @version 1.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/vm/mappedrom.h>

namespace Kernel 
{
    /// @brief: map_bios + ebda
    MappedROM map_bios();
    MappedROM map_ebda();

} // namespace Kernel
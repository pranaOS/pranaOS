/**
 * @file mmio.cpp
 * @author Krisna Pranav
 * @brief MMIO
 * @version 6.0
 * @date 2024-10-24
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/prekernel/arch/aarch64/mmio.h>
#include <kernel/prekernel/arch/aarch64/mainidregister.h>

namespace Prekernel 
{
    /**
     * @brief Construct a new MMIO::MMIO object
     * 
     */
    MMIO::MMIO()
        : m_base_address(0xFE00'0000)
    {
        MainIdRegister id;
        if (id.part_num() <= MainIdRegister::RaspberryPi3)
            m_base_address = 0x3F00'0000;
    }

    /**
     * @return MMIO& 
     */
    MMIO& MMIO::the()
    {
        static MMIO instance;
        return instance;
    }
} // namespace Prekernel

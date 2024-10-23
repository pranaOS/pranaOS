/**
 * @file mainidregister.cpp
 * @author Krisna Pranav
 * @brief Main ID Register
 * @version 6.0
 * @date 2024-10-23
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/prekernel/arch/aarch64/mainidregister.h>

namespace Prekernel 
{

    /**
     * @brief Construct a new MainIdRegister::MainIdRegister object
     * 
     */
    MainIdRegister::MainIdRegister()
    {
        unsigned int mrs;

        asm volatile("mrs %x0, MIDR_EL1"
                    : "=r"(mrs));

        m_value = mrs;
    }

}

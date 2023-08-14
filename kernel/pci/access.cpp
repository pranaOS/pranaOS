/**
 * @file access.cpp
 * @author Krisna Pranav
 * @brief access
 * @version 1.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/io.h>
#include <kernel/pci/ioaccess.h>
#include <kernel/pci/access.h>

namespace Kernel
{
    namespace PCI
    {
        static Access* s_access;

        /**
         * @param address 
         * @param field 
         * @param value 
         */
        inline void write8(Address address, u32 field, u8 value)
        {
            Access::the().write8_field(address, field, value);
        }

        /**
         * @param address 
         * @param field 
         * @param value 
         */
        inline void write16(Address address, u32 field, u8 value)
        {
            Access::the().write16_field(address, field, value);
        }

        /**
         * @param address 
         * @param field 
         * @param value 
         */
        inline void write32(Address address, u32 field, u8 value)
        {
            Access::the().write32_field(address, field, value);
        }

        /**
         * @return Access& 
         */
        Access& Access::the()
        {
            if (s_access == nullptr) {
                ASSERT_NOT_REACHED();
            }

            return *s_access;
        }

        bool Access::is_initialized()
        {
            return (s_access != nullptr);
        }
    }
}
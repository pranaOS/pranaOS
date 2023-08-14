/**
 * @file access.h
 * @author Krisna Pranav
 * @brief access
 * @version 1.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/string.h>
#include <mods/vector.h>
#include <kernel/pci/definitions.h>

namespace Kernel 
{

    class PCI::Access
    {
    public:
        enum class Type
        {
            IO,
            MMIO,
        };

        void enumerate(Function<void(Address, ID)>&) const;

    private:
        /// @brief: enumerate_hardware
        virtual void enumerate_hardware(Function<void(Address, ID)>) = 0;

        /**
         * @param address 
         * @param field 
         * @return u8 
         */
        u8 early_read8_field(Address address, u32 field);

        /**
         * @param address 
         * @param field 
         * @return u16 
         */
        u16 early_read16_field(Address address, u32 field);

        /**
         * @param address 
         * @param field 
         * @return u32 
         */
        u32 early_read32_field(Address address, u32 field);

        /**
         * @param address 
         * @return u16 
         */
        u16 early_read_type(Address address);

        Access();

        Vector<PhysicalID> m_physical_ids;
    }; // class PCI

} // namespace Kernel
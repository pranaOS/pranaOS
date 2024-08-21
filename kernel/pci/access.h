/**
 * @file access.h
 * @author Krisna Pranav
 * @brief access
 * @version 6.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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
        
        /// @brief: enumerate
        void enumerate(Function<void(Address, ID)>&) const;

        /**
         * @param type 
         * @param bus 
         */
        void enumerate_bus(int type, u8 bus, Function<void(Address, ID)>&);

        /**
         * @param type 
         * @param bus 
         * @param slot 
         * @param function 
         * @param callback 
         */
        void enumerate_functions(int type, u8 bus, u8 slot, u8 function, Function<void(Address, ID)>& callback);

        /**
         * @param type 
         * @param bus 
         * @param slot 
         * @param callback 
         */
        void enumerate_slot(int type, u8 bus, u8 slot, Function<void(Address, ID)>& callback);

        /**
         * @return Access& 
         */
        static Access& the();

        /// @brief: is_initialized
        static bool is_initialized();
        
        /**
         * @return uint32_t 
         */
        virtual uint32_t segment_count() const = 0;

        /**
         * @param segment 
         * @return uint8_t 
         */
        virtual uint8_t segment_start_bus(u32 segment) const = 0;

        /**
         * @param segment 
         * @return uint8_t 
         */
        virtual uint8_t segment_end_bus(u32 segment) const = 0;

        /**
         * @return const char* 
         */
        virtual const char* access_type() const = 0;

        /**
         * @param address 
         * @param field 
         * @param value 
         */
        virtual void write8_field(Address address, u32 field, u8 value) = 0;

        /**
         * @param address 
         * @param field 
         * @param value 
         */
        virtual void write16_field(Address address, u32 field, u16 value) = 0;

        /**
         * @param address 
         * @param field 
         * @param value 
         */
        virtual void write32_field(Address address, u32 field, u32 value) = 0;

        /** 
         * @param address 
         * @param field 
         * @return u8 
         */
        virtual u8 read8_field(Address address, u32 field) = 0;

        /**
         * @param address 
         * @param field 
         * @return u16 
         */
        virtual u16 read16_field(Address address, u32 field) = 0;

        /**
         * @param address 
         * @param field 
         * @return u32 
         */
        virtual u32 read32_field(Address address, u32 field) = 0;

    protected:

        /**
         * @brief enumerate_hardware
         * 
         */
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

/**
 * @file access.cpp
 * @author Krisna Pranav
 * @brief access
 * @version 6.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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
        inline void write16(Address address, u32 field, u16 value) 
        { 
            Access::the().write16_field(address, field, value); 
        }

        /**
         * @param address 
         * @param field 
         * @param value 
         */
        inline void write32(Address address, u32 field, u32 value) 
        { 
            Access::the().write32_field(address, field, value); 
        }

        /**
         * @param address 
         * @param field 
         * @return u8 
         */
        inline u8 read8(Address address, u32 field) 
        { 
            return Access::the().read8_field(address, field); 
        }

        /**
         * @param address 
         * @param field 
         * @return u16 
         */
        inline u16 read16(Address address, u32 field) 
        { 
            return Access::the().read16_field(address, field); 
        }

        /**
         * @param address 
         * @param field 
         * @return u32 
         */
        inline u32 read32(Address address, u32 field) 
        { 
            return Access::the().read32_field(address, field); 
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

        /**
         * @return true 
         * @return false 
         */
        bool Access::is_initialized()
        {
            return (s_access != nullptr);
        }

        /// @brief Construct a new Access::Access object
        Access::Access()
        {
            s_access = this;
        }

        /**
         * @param address 
         * @param field 
         * @return u8 
         */
        u8 Access::early_read8_field(Address address, u32 field)
        {
            IO::out32(PCI_ADDRESS_PORT, address.io_address_for_field(field));
            return IO::in8(PCI_VALUE_PORT + (field & 3));
        }

        /**
         * @param address 
         * @param field 
         * @return u16 
         */
        u16 Access::early_read16_field(Address address, u32 field)
        {
            IO::out32(PCI_ADDRESS_PORT, address.io_address_for_field(field));
            return IO::in16(PCI_VALUE_PORT + (field & 2));
        }

        /**
         * @param address 
         * @param field 
         * @return u32 
         */
        u32 Access::early_read32_field(Address address, u32 field)
        {
            IO::out32(PCI_ADDRESS_PORT, address.io_address_for_field(field));
            return IO::in32(PCI_VALUE_PORT);
        }

        /**
         * @param address 
         * @return u16 
         */
        u16 Access::early_read_type(Address address)
        {
            return (early_read8_field(address, PCI_CLASS) << 8u) | early_read8_field(address, PCI_SUBCLASS);
        }

        /**
         * @param type 
         * @param bus 
         * @param slot 
         * @param function 
         * @param callback 
         */
        void Access::enumerate_functions(int type, u8 bus, u8 slot, u8 function, Function<void(Address, ID)>& callback)
        {
            Address address(0, bus, slot, function);

            if (type == -1 || type == early_read_type(address))
                callback(address, { early_read16_field(address, PCI_VENDOR_ID), early_read16_field(address, PCI_DEVICE_ID) });

            if (early_read_type(address) == PCI_TYPE_BRIDGE) {
                u8 secondary_bus = early_read8_field(address, PCI_SECONDARY_BUS);
        #ifdef PCI_DEBUG
                klog() << "PCI: Found secondary bus: " << secondary_bus;
        #endif

                ASSERT(secondary_bus != bus);

                enumerate_bus(type, secondary_bus, callback);
            }
        }

        /**
         * @param type 
         * @param bus 
         * @param slot 
         * @param callback 
         */
        void Access::enumerate_slot(int type, u8 bus, u8 slot, Function<void(Address, ID)>& callback)
        {
            Address address(0, bus, slot, 0);

            if (early_read16_field(address, PCI_VENDOR_ID) == PCI_NONE)
                return;

            enumerate_functions(type, bus, slot, 0, callback);

            if (!(early_read8_field(address, PCI_HEADER_TYPE) & 0x80))
                return;

            for (u8 function = 1; function < 8; ++function) {
                Address address(0, bus, slot, function);
                if (early_read16_field(address, PCI_VENDOR_ID) != PCI_NONE)
                    enumerate_functions(type, bus, slot, function, callback);
            }
        }

        /**
         * @param type 
         * @param bus 
         * @param callback 
         */
        void Access::enumerate_bus(int type, u8 bus, Function<void(Address, ID)>& callback)
        {
            for (u8 slot = 0; slot < 32; ++slot)
                enumerate_slot(type, bus, slot, callback);
        }

        /**
         * @param callback 
         */
        void Access::enumerate(Function<void(Address, ID)>& callback) const
        {
            for (auto& physical_id : m_physical_ids) {
                callback(physical_id.address(), physical_id.id());
            }
        }

        /**
         * @param callback 
         */
        void enumerate(Function<void(Address, ID)> callback)
        {
            Access::the().enumerate(callback);
        }

        /**
         * @param address 
         * @param field 
         * @param access_size 
         * @param value 
         */
        void raw_access(Address address, u32 field, size_t access_size, u32 value)
        {
            ASSERT(access_size != 0);

            if (access_size == 1) {
                write8(address, field, value);
                return;
            }
            if (access_size == 2) {
                write16(address, field, value);
                return;
            }
            if (access_size == 4) {
                write32(address, field, value);
                return;
            }

            ASSERT_NOT_REACHED();
        }

        /**
         * @param address 
         * @return ID 
         */
        ID get_id(Address address)
        {
            return { read16(address, PCI_VENDOR_ID), read16(address, PCI_DEVICE_ID) };
        }

        /**
         * @param address 
         */
        void enable_interrupt_line(Address address)
        {
            write16(address, PCI_COMMAND, read16(address, PCI_COMMAND) & ~(1 << 10));
        }

        /**
         * @param address 
         */
        void disable_interrupt_line(Address address)
        {
            write16(address, PCI_COMMAND, read16(address, PCI_COMMAND) | 1 << 10);
        }

        /**
         * @param address 
         * @return u8 
         */
        u8 get_interrupt_line(Address address)
        {
            return read8(address, PCI_INTERRUPT_LINE);
        }

        /**
         * @param address 
         * @return u32 
         */
        u32 get_BAR0(Address address)
        {
            return read32(address, PCI_BAR0);
        }

        /**
         * @param address 
         * @return u32 
         */
        u32 get_BAR1(Address address)
        {
            return read32(address, PCI_BAR1);
        }

        /**
         * @param address 
         * @return u32 
         */
        u32 get_BAR2(Address address)
        {
            return read32(address, PCI_BAR2);
        }

        /**
         * @param address 
         * @return u32 
         */
        u32 get_BAR3(Address address)
        {
            return read16(address, PCI_BAR3);
        }

        /**
         * @param address 
         * @return u32 
         */
        u32 get_BAR4(Address address)
        {
            return read32(address, PCI_BAR4);
        }

        /**
         * @param address 
         * @return u32 
         */
        u32 get_BAR5(Address address)
        {
            return read32(address, PCI_BAR5);
        }

        /**
         * @param address 
         * @return u8 
         */
        u8 get_revision_id(Address address)
        {
            return read8(address, PCI_REVISION_ID);
        }

        /**
         * @param address 
         * @return u8 
         */
        u8 get_subclass(Address address)
        {
            return read8(address, PCI_SUBCLASS);
        }

        /**
         * @param address 
         * @return u8 
         */
        u8 get_class(Address address)
        {
            return read8(address, PCI_CLASS);
        }

        /**
         * @param address 
         * @return u8 
         */
        u8 get_programming_interface(Address address)
        {
            return read8(address, PCI_PROG_IF);
        }
        
        /**
         * @param address 
         * @return u16 
         */
        u16 get_subsystem_id(Address address)
        {
            return read16(address, PCI_SUBSYSTEM_ID);
        }

        /**
         * @param address 
         * @return u16 
         */
        u16 get_subsystem_vendor_id(Address address)
        {
            return read16(address, PCI_SUBSYSTEM_VENDOR_ID);
        }

        /**
         * @param address 
         */
        void enable_bus_mastering(Address address)
        {
            auto value = read16(address, PCI_COMMAND);

            value |= (1 << 2);
            value |= (1 << 0);

            write16(address, PCI_COMMAND, value);
        }

        /**
         * @param address 
         */
        void disable_bus_mastering(Address address)
        {
            auto value = read16(address, PCI_COMMAND);

            value &= ~(1 << 2);
            value |= (1 << 0);

            write16(address, PCI_COMMAND, value);
        }

        /**
         * @param address 
         * @param bar_number 
         * @return size_t 
         */
        size_t get_BAR_space_size(Address address, u8 bar_number)
        {
            ASSERT(bar_number < 6);

            u8 field = (PCI_BAR0 + (bar_number << 2));
            u32 bar_reserved = read32(address, field);

            write32(address, field, 0xFFFFFFFF);

            u32 space_size = read32(address, field);

            write32(address, field, bar_reserved);

            space_size &= 0xfffffff0;
            space_size = (~space_size) + 1;

            return space_size;
        }
    } // namespace PCI
} // namespace Kernel

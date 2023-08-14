/**
 * @file definitions.h
 * @author Krisna Pranav
 * @brief definitions
 * @version 1.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 


#include <mods/function.h>
#include <mods/logstream.h>
#include <mods/types.h>

namespace Kernel 
{

    #define PCI_VENDOR_ID 0x00            
    #define PCI_DEVICE_ID 0x02            
    #define PCI_COMMAND 0x04              
    #define PCI_STATUS 0x06               
    #define PCI_REVISION_ID 0x08          
    #define PCI_PROG_IF 0x09              
    #define PCI_SUBCLASS 0x0a             
    #define PCI_CLASS 0x0b                
    #define PCI_CACHE_LINE_SIZE 0x0c      
    #define PCI_LATENCY_TIMER 0x0d        
    #define PCI_HEADER_TYPE 0x0e          
    #define PCI_BIST 0x0f                 
    #define PCI_BAR0 0x10                 
    #define PCI_BAR1 0x14                 
    #define PCI_BAR2 0x18                 
    #define PCI_BAR3 0x1C                 
    #define PCI_BAR4 0x20                 
    #define PCI_BAR5 0x24                 
    #define PCI_SUBSYSTEM_ID 0x2C         
    #define PCI_SUBSYSTEM_VENDOR_ID 0x2E  
    #define PCI_CAPABILITIES_POINTER 0x34 
    #define PCI_INTERRUPT_LINE 0x3C       
    #define PCI_SECONDARY_BUS 0x19        
    #define PCI_HEADER_TYPE_DEVICE 0
    #define PCI_HEADER_TYPE_BRIDGE 1
    #define PCI_TYPE_BRIDGE 0x0604
    #define PCI_ADDRESS_PORT 0xCF8
    #define PCI_VALUE_PORT 0xCFC
    #define PCI_NONE 0xFFFF
    #define PCI_MAX_DEVICES_PER_BUS 32
    #define PCI_MAX_BUSES 256
    #define PCI_MAX_FUNCTIONS_PER_DEVICE 8

    namespace PCI 
    {
        struct ID 
        {
            u16 vendor_id { 0 };
            u16 device_id { 0 };

            /**
             * @return true 
             * @return false 
             */
            bool is_null() const 
            { 
                return !vendor_id && !device_id; 
            }

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator==(const ID& other) const
            {
                return vendor_id == other.vendor_id && device_id == other.device_id;
            }

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator!=(const ID& other) const
            {
                return vendor_id != other.vendor_id || device_id != other.device_id;
            }

        }; // struct ID

        /**
         * @param stream 
         * @param value 
         * @return const LogStream& 
         */
        inline const LogStream& operator<<(const LogStream& stream, const ID value)
        {
            return stream << "(" << String::format("%w", value.vendor_id) << ":" << String::format("%w", value.device_id) << ")";
        }

        struct Address 
        {
        public:
            Address() { }

            /**
             * @param seg 
             */
            Address(u16 seg)
                : m_seg(seg)
                , m_bus(0)
                , m_slot(0)
                , m_function(0)
            {
            }

            /**
             * @param seg 
             * @param bus 
             * @param slot 
             * @param function 
             */
            Address(u16 seg, u8 bus, u8 slot, u8 function)
                : m_seg(seg)
                , m_bus(bus)
                , m_slot(slot)
                , m_function(function)
            {
            }
            
            /**
             * @param address 
             */
            Address(const Address& address)
                : m_seg(address.seg())
                , m_bus(address.bus())
                , m_slot(address.slot())
                , m_function(address.function())
            {
            }

            /**
             * @return true 
             * @return false 
             */
            bool is_null() const 
            { 
                return !m_bus && !m_slot && !m_function; 
            }

            /**
             * @return true 
             * @return false 
             */
            operator bool() const 
            { 
                return !is_null(); 
            }

            /**
             * @return true 
             * @return false 
             */
            bool operator==(const Address&) const = delete;
            bool operator<=(const Address&) const = delete;
            bool operator>=(const Address&) const = delete;
            bool operator<(const Address&) const = delete;
            bool operator>(const Address&) const = delete;

            /**
             * @return u16 
             */
            u16 seg() const 
            { 
                return m_seg; 
            }

            /**
             * @return u8 
             */
            u8 bus() const 
            { 
                return m_bus; 
            }

            u8 slot() const 
            { 
                return m_slot; 
            }

            u8 function() const 
            { 
                return m_function; 
            }

            /**
             * @param field 
             * @return u32 
             */
            u32 io_address_for_field(u8 field) const
            {
                return 0x80000000u | (m_bus << 16u) | (m_slot << 11u) | (m_function << 8u) | (field & 0xfc);
            }

        protected:
            u32 m_seg { 0 };
            u8 m_bus { 0 };
            u8 m_slot { 0 };
            u8 m_function { 0 };
        };

        /**
         * @param stream 
         * @param value 
         * @return const LogStream& 
         */
        inline const LogStream& operator<<(const LogStream& stream, const Address value)
        {
            return stream << "PCI [" << String::format("%w", value.seg()) << ":" << String::format("%b", value.bus()) << ":" << String::format("%b", value.slot()) << "." << String::format("%b", value.function()) << "]";
        }

        struct ChangeableAddress : public Address 
        {
            /// @brief Construct a new ChangeableAddress object
            ChangeableAddress()
                : Address(0)
            {
            }

            /**
             * @param seg 
             */
            explicit ChangeableAddress(u16 seg)
                : Address(seg)
            {
            }

            /**
             * @param seg 
             * @param bus 
             * @param slot 
             * @param function 
             */
            ChangeableAddress(u16 seg, u8 bus, u8 slot, u8 function)
                : Address(seg, bus, slot, function)
            {
            }

            /**
             * @param seg 
             */
            void set_seg(u16 seg) 
            { 
                m_seg = seg; 
            }

            /**
             * @param bus 
             */
            void set_bus(u8 bus) 
            { 
                m_bus = bus; 
            }

            /**
             * @param slot 
             */
            void set_slot(u8 slot) 
            { 
                m_slot = slot; 
            }

            /**
             * @param function 
             */
            void set_function(u8 function) 
            { 
                m_function = function; 
            }

            /**
             * @param address 
             * @return true 
             * @return false 
             */
            bool operator==(const Address& address)
            {
                if (m_seg == address.seg() && m_bus == address.bus() && m_slot == address.slot() && m_function == address.function())
                    return true;
                else
                    return false;
            }

            /**
             * @param address 
             * @return const ChangeableAddress& 
             */
            const ChangeableAddress& operator=(const Address& address)
            {
                set_seg(address.seg());
                set_bus(address.bus());
                set_slot(address.slot());
                set_function(address.function());
                return *this;
            }
        };

        class PhysicalID 
        {
        public:
            /**
             * @param address 
             * @param id 
             */
            PhysicalID(Address address, ID id)
                : m_address(address)
                , m_id(id)
            {
            }

            /**
             * @return const ID& 
             */
            const ID& id() const 
            { 
                return m_id; 
            }

            /**
             * @return const Address& 
             */
            const Address& address() const 
            { 
                return m_address; 
            }

        private:
            Address m_address;
            ID m_id;
        }; // class PhysicalID

        /**
         * @return ID 
         */
        ID get_id(PCI::Address);

        /**
         * @param callback 
         */
        void enumerate(Function<void(Address, ID)> callback);

        void enable_interrupt_line(Address);
        void disable_interrupt_line(Address);

        /**
         * @return u8 
         */
        u8 get_interrupt_line(Address);

        void raw_access(Address, u32, size_t, u32);

        /**
         * @return u32 
         */
        u32 get_BAR0(Address);
        u32 get_BAR1(Address);
        u32 get_BAR2(Address);
        u32 get_BAR3(Address);
        u32 get_BAR4(Address);
        u32 get_BAR5(Address);

        /**
         * @return u8 
         */
        u8 get_revision_id(Address);
        u8 get_programming_interface(Address);
        u8 get_subclass(Address);
        u8 get_class(Address);

        /**
         * @return u16 
         */
        u16 get_subsystem_id(Address);
        u16 get_subsystem_vendor_id(Address);

        /**
         * @return size_t 
         */
        size_t get_BAR_space_size(Address, u8);
        void enable_bus_mastering(Address);
        void disable_bus_mastering(Address);

        class Access;
        class MMIOAccess;
        class IOAccess;
        class MMIOSegment;
        class Device;

    } // namespace PCI

} // namespace Kernel

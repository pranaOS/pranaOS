/**
 * @file definitions.h
 * @author Krisna Pranav
 * @brief Definitions.h
 * @version 6.0
 * @date 2024-10-12
 * 
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/badge.h>
#include <mods/distinctnums.h>
#include <mods/function.h>
#include <mods/types.h>
#include <mods/vector.h>
#include <kernel/physical_address.h>
#include <kernel/debug.h>


namespace Kernel 
{

    namespace PCI 
    {

        enum class HeaderType 
        {
            Device = 0,
            Bridge = 1,
        }; // enum class HeaderType

        enum class RegisterOffset 
        {
            VENDOR_ID = 0x00,            
            DEVICE_ID = 0x02,            
            COMMAND = 0x04,              
            STATUS = 0x06,               
            REVISION_ID = 0x08,          
            PROG_IF = 0x09,              
            SUBCLASS = 0x0a,             
            CLASS = 0x0b,                
            CACHE_LINE_SIZE = 0x0c,      
            LATENCY_TIMER = 0x0d,        
            HEADER_TYPE = 0x0e,          
            BIST = 0x0f,                 
            BAR0 = 0x10,                 
            BAR1 = 0x14,                 
            BAR2 = 0x18,                 
            SECONDARY_BUS = 0x19,        
            BAR3 = 0x1C,                 
            BAR4 = 0x20,                 
            BAR5 = 0x24,                 
            SUBSYSTEM_VENDOR_ID = 0x2C,  
            SUBSYSTEM_ID = 0x2E,         
            CAPABILITIES_POINTER = 0x34, 
            INTERRUPT_LINE = 0x3C,       
            INTERRUPT_PIN = 0x3D,        
        }; // enum class RegisterOffset

        enum class Limits 
        {
            MaxDevicesPerBus = 32,
            MaxBusesPerDomain = 256,
            MaxFunctionsPerDevice = 8,
        }; // enum class Limits

        static constexpr u16 address_port = 0xcf8;
        static constexpr u16 value_port = 0xcfc;

        static constexpr size_t mmio_device_space_size = 4096;
        static constexpr u16 none_value = 0xffff;
        static constexpr size_t memory_range_per_bus = mmio_device_space_size * to_underlying(Limits::MaxFunctionsPerDevice) * to_underlying(Limits::MaxDevicesPerBus);

        enum class ClassID 
        {
            MassStorage = 0x1,
            Multimedia = 0x4,
            Bridge = 0x6,
        }; // enum class ClassID

        namespace MassStorage 
        {

            enum class SubclassID 
            {
                IDEController = 0x1,
                SATAController = 0x6,
                NVMeController = 0x8,
            }; // enum class SubclassId

            enum class SATAProgIF 
            {
                AHCI = 0x1,
            }; // enum class SATAProgIf

        } // namespace MassStorage

        namespace Multimedia 
        {

            enum class SubclassID 
            {
                AudioController = 0x1,
            }; // enum class SubclassID

        } // namespace Multimedia

        namespace Bridge 
        {

            enum class SubclassID 
            {
                PCI_TO_PCI = 0x4,
            }; // enum class SubclassId

        } // namespace Bridge

        TYPEDEF_DISTINCT_ORDERED_ID(u8, CapabilityID);
        namespace Capabilities 
        {
            enum ID 
            {
                Null = 0x0,
                MSI = 0x5,
                VendorSpecific = 0x9,
                MSIX = 0x11,
            }; // enum ID
        } // namespace Capabilities

        struct HardwareID 
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
            bool operator==(const HardwareID& other) const
            {
                return vendor_id == other.vendor_id && device_id == other.device_id;
            }

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator!=(const HardwareID& other) const
            {
                return vendor_id != other.vendor_id || device_id != other.device_id;
            }
        }; // struct HardwareID

        class Domain 
        {
        public:
            /**
             * @brief Construct a new Domain object
             * 
             */
            Domain() = delete;

            /**
             * @brief Construct a new Domain object
             * 
             * @param domain_number 
             * @param start_bus 
             * @param end_bus 
             */
            Domain(u32 domain_number, u8 start_bus, u8 end_bus)
                : m_domain_number(domain_number)
                , m_start_bus(start_bus)
                , m_end_bus(end_bus)
            {
            }

            /**
             * @return u8 
             */
            u8 start_bus() const 
            { 
                return m_start_bus; 
            }

            /**
             * @return u8 
             */
            u8 end_bus() const 
            { 
                return m_end_bus; 
            }

            u32 domain_number() const 
            { 
                return m_domain_number; 
            }

        private:
            u32 m_domain_number;
            u8 m_start_bus;
            u8 m_end_bus;
        }; // class Domain

        struct Address 
        {
        public:
            /**
             * @brief Construct a new Address object
             * 
             */
            Address() = default;

            /**
             * @brief Construct a new Address object
             * 
             * @param domain 
             */
            Address(u32 domain)
                : m_domain(domain)
                , m_bus(0)
                , m_device(0)
                , m_function(0)
            {
            }

            /**
             * @brief Construct a new Address object
             * 
             * @param domain 
             * @param bus 
             * @param device 
             * @param function 
             */
            Address(u32 domain, u8 bus, u8 device, u8 function)
                : m_domain(domain)
                , m_bus(bus)
                , m_device(device)
                , m_function(function)
            {
            }

            /**
             * @brief Construct a new Address object
             * 
             * @param address 
             */
            Address(const Address& address) = default;

            bool is_null() const 
            { 
                return !m_bus && !m_device && !m_function; 
            }

            operator bool() const 
            { 
                return !is_null(); 
            }


            bool operator<=(const Address&) const = delete;
            bool operator>=(const Address&) const = delete;
            bool operator<(const Address&) const = delete;
            bool operator>(const Address&) const = delete;

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator==(const Address& other) const
            {
                if (this == &other)
                    return true;

                return m_domain == other.m_domain && m_bus == other.m_bus && m_device == other.m_device && m_function == other.m_function;
            }

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator!=(const Address& other) const
            {
                return !(*this == other);
            }

            u32 domain() const 
            { 
                return m_domain; 
            }

            u8 bus() const 
            { 
                return m_bus; 
            }

            u8 device() const 
            { 
                return m_device; 
            }

            u8 function() const 
            { 
                return m_function; 
            }

        private:
            u32 m_domain { 0 };
            u8 m_bus { 0 };
            u8 m_device { 0 };
            u8 m_function { 0 };
        }; // struct Address

        class Capability 
        {
        public:
            /**
             * @brief Construct a new Capability object
             * 
             * @param address 
             * @param id 
             * @param ptr 
             */
            Capability(const Address& address, u8 id, u8 ptr)
                : m_address(address)
                , m_id(id)
                , m_ptr(ptr)
            {
            }

            /**
             * @brief 
             * 
             * @return CapabilityID 
             */
            CapabilityID id() const 
            { 
                return m_id; 
            }
            
            /**
             * @return u8 
             */
            u8 read8(u32) const;

            /**
             * @return u16 
             */
            u16 read16(u32) const;

            /**
             * @return u32 
             */
            u32 read32(u32) const;

            void write8(u32, u8);
            void write16(u32, u16);
            void write32(u32, u32);

        private:
            Address m_address;
            const CapabilityID m_id;
            const u8 m_ptr;
        }; // class Capability 

        TYPEDEF_DISTINCT_ORDERED_ID(u8, ClassCode);
        TYPEDEF_DISTINCT_ORDERED_ID(u8, SubclassCode);
        TYPEDEF_DISTINCT_ORDERED_ID(u8, ProgrammingInterface);
        TYPEDEF_DISTINCT_ORDERED_ID(u8, RevisionID);
        TYPEDEF_DISTINCT_ORDERED_ID(u16, SubsystemID);
        TYPEDEF_DISTINCT_ORDERED_ID(u16, SubsystemVendorID);
        TYPEDEF_DISTINCT_ORDERED_ID(u8, InterruptLine);
        TYPEDEF_DISTINCT_ORDERED_ID(u8, InterruptPin);

        class Access;

        class DeviceIdentifier 
        {
        public:
            /**
             * @brief Construct a new Device Identifier object
             * 
             * @param address 
             * @param hardware_id 
             * @param revision_id 
             * @param class_code 
             * @param subclass_code 
             * @param prog_if 
             * @param subsystem_id 
             * @param subsystem_vendor_id 
             * @param interrupt_line 
             * @param interrupt_pin 
             * @param capabilities 
             */
            DeviceIdentifier(Address address, HardwareID hardware_id, RevisionID revision_id, ClassCode class_code, SubclassCode subclass_code, ProgrammingInterface prog_if, SubsystemID subsystem_id, SubsystemVendorID subsystem_vendor_id, InterruptLine interrupt_line, InterruptPin interrupt_pin, Vector<Capability> const& capabilities)
                : m_address(address)
                , m_hardware_id(hardware_id)
                , m_revision_id(revision_id)
                , m_class_code(class_code)
                , m_subclass_code(subclass_code)
                , m_prog_if(prog_if)
                , m_subsystem_id(subsystem_id)
                , m_subsystem_vendor_id(subsystem_vendor_id)
                , m_interrupt_line(interrupt_line)
                , m_interrupt_pin(interrupt_pin)
                , m_capabilities(capabilities)
            {
                if constexpr (PCI_DEBUG) {
                    for (const auto& capability : capabilities)
                        dbgln("{} has capability {}", address, capability.id());
                }
            }

            /**
             * @return Vector<Capability> const& 
             */
            Vector<Capability> const& capabilities() const 
            { 
                return m_capabilities; 
            }

            /**
             * @return HardwareID const& 
             */
            HardwareID const& hardware_id() const 
            { 
                return m_hardware_id; 
            }

            /**
             * @return Address const& 
             */
            Address const& address() const 
            { 
                return m_address; 
            }

            /**
             * @return RevisionID 
             */
            RevisionID revision_id() const 
            { 
                return m_revision_id; 
            }

            /**
             * @return ClassCode 
             */
            ClassCode class_code() const 
            { 
                return m_class_code; 
            }

            /**
             * @return SubclassCode 
             */
            SubclassCode subclass_code() const 
            { 
                return m_subclass_code; 
            }

            /**
             * @return ProgrammingInterface 
             */
            ProgrammingInterface prog_if() const 
            { 
                return m_prog_if; 
            }
            
            /**
             * @return SubsystemID 
             */
            SubsystemID subsystem_id() const 
            { 
                return m_subsystem_id; 
            }

            /**
             * @return SubsystemVendorID 
             */
            SubsystemVendorID subsystem_vendor_id() const 
            { 
                return m_subsystem_vendor_id; 
            }

            /**
             * @return InterruptLine 
             */
            InterruptLine interrupt_line() const 
            { 
                return m_interrupt_line; 
            }

            /**
             * @return InterruptPin 
             */
            InterruptPin interrupt_pin() const 
            { 
                return m_interrupt_pin; 
            }

            /**
             * @param new_subclass 
             */
            void apply_subclass_code_change(Badge<Access>, SubclassCode new_subclass)
            {
                m_subclass_code = new_subclass;
            }

            /**
             * @param new_progif 
             */
            void apply_prog_if_change(Badge<Access>, ProgrammingInterface new_progif)
            {
                m_prog_if = new_progif;
            }

        private:
            Address m_address;
            HardwareID m_hardware_id;

            RevisionID m_revision_id;
            ClassCode m_class_code;
            SubclassCode m_subclass_code;
            ProgrammingInterface m_prog_if;
            SubsystemID m_subsystem_id;
            SubsystemVendorID m_subsystem_vendor_id;

            InterruptLine m_interrupt_line;
            InterruptPin m_interrupt_pin;

            Vector<Capability> m_capabilities;
        }; // class DeviceIdentifier

        class Domain;
        class Device;
    } // namespace PCI
} // namespace Kernel

template<>
struct Mods::Formatter<Kernel::PCI::Address> : Formatter<FormatString> 
{
    /**
     * @param builder 
     * @param value 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> format(FormatBuilder& builder, Kernel::PCI::Address value)
    {
        return Formatter<FormatString>::format(
            builder,
            "[{:04x}:{:02x}:{:02x}:{:02x}]", value.domain(), value.bus(), value.device(), value.function());
    }
};

template<>
struct Mods::Formatter<Kernel::PCI::HardwareID> : Formatter<FormatString> 
{
    /**
     * @param builder 
     * @param value 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> format(FormatBuilder& builder, Kernel::PCI::HardwareID value)
    {
        return Formatter<FormatString>::format(
            builder,
            "HardwareID [{:04x}:{:04x}]", value.vendor_id, value.device_id);
    }
};

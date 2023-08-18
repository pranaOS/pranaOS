/**
 * @file ioaccess.h
 * @author Krisna Pranav
 * @brief io access
 * @version 6.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/pci/access.h>

namespace Kernel 
{
    namespace PCI 
    {
        class IOAccess final : public PCI::Access 
        {
        public:

            /// @breif: initialize
            static void initialize();

        protected:
            /// @brief Construct a new IOAccess object
            IOAccess();

        private:
            /// @brief: enumerate hardware
            virtual void enumerate_hardware(Function<void(Address, ID)>) override;

            /**
             * @return const char* 
             */
            virtual const char* access_type() const override 
            { 
                return "IO-Access"; 
            };

            /**
             * @return uint32_t 
             */
            virtual uint32_t segment_count() const override 
            { 
                return 1; 
            };

            /**
             * @param address 
             */
            virtual void write8_field(Address address, u32, u8) override final;

            /**
             * @param address 
             */
            virtual void write16_field(Address address, u32, u16) override final;

            /**
             * @param address 
             */
            virtual void write32_field(Address address, u32, u32) override final;

            /**
             * @param address 
             * @return u8 
             */
            virtual u8 read8_field(Address address, u32) override;

            /**
             * @param address 
             * @return u16 
             */
            virtual u16 read16_field(Address address, u32) override;

            /**
             * @param address 
             * @return u32 
             */
            virtual u32 read32_field(Address address, u32) override;

            /**
             * @return uint8_t 
             */
            virtual uint8_t segment_start_bus(u32) const override 
            { 
                return 0x0; 
            }

            /**
             * @return uint8_t 
             */
            virtual uint8_t segment_end_bus(u32) const override 
            { 
                return 0xFF; 
            }
        }; // class IOAccess
    } // namespace PCI 
} // namespace Kernel

/**
 * @file ioaccess.h
 * @author Krisna Pranav
 * @brief io access
 * @version 1.0
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
        protected:
            IOAccess();

        private:

            /// enumerate_hardware
            virtual void enumerate_hardware(Function<void(Address, ID)>) override;

            /**
             * @return const char* 
             */
            virtual const char* access_type() const override
            {
                return "IO-Access";
            }

            /**
             * @return uint32_t 
             */
            virtual uint32_t segment_count() const override
            {
                return 1;
            }
        }; // class IOAccess 

    } // namespace PCI

    
} // namespace Kernel
/**
 * @file device.h
 * @author Krisna Pranav
 * @brief device
 * @version 1.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/types.h>
#include <kernel/interrupt/irqhandler.h>
#include <kernel/pci/definitions.h>

namespace Kernel
{

    class PCI::Device : public IRQHandler
    {
    public:
        /**
         * @return Address 
         */
        Address pci_address() const
        {
            return m_pci_address;
        }

    private:
        Address m_pci_address;
    }; // class PCI::Device

} // namespace Kernel
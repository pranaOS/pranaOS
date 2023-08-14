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
        };

    protected:
        /**
         * @param pci_address 
         */
        Device(Address pci_address);

        /**
         * @param pci_address 
         * @param interrupt_vector 
         */
        Device(Address pci_address, u8 interrupt_vector);

        /// @brief Destroy the Device object
        ~Device();

    private:
        Address m_pci_address;
    }; // class PCI::Device

} // namespace Kernel

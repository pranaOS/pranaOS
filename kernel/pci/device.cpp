/**
 * @file device.cpp
 * @author Krisna Pranav
 * @brief device
 * @version 6.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/pci/device.h>

namespace Kernel 
{
    namespace PCI 
    {
        /**
         * @param address 
         */
        Device::Device(Address address)
            : IRQHandler(get_interrupt_line(address))
            , m_pci_address(address)
        { }

        /**
         * @param address 
         * @param interrupt_vector 
         */
        Device::Device(Address address, u8 interrupt_vector)
            : IRQHandler(interrupt_vector)
            , m_pci_address(address)
        { }

        /// @brief Destroy the Device::Device object
        Device::~Device()
        { }

    } // namespace PCI
} // namespace Kernel

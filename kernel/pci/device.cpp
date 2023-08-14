/**
 * @file device.cpp
 * @author Krisna Pranav
 * @brief device
 * @version 1.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
        {}

        /// @brief Destroy the Device::Device object
        Device::~Device()
        {}
    }
}
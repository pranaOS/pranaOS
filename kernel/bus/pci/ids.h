/**
 * @file ids.h
 * @author Krisna Pranav
 * @brief IDS
 * @version 6.0
 * @date 2024-10-13
 * 
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Kernel::PCI 
{

    enum VendorID 
    {
        VirtIO = 0x1af4,
        Intel = 0x8086,
        WCH = 0x1c00,
        RedHat = 0x1b36,
        Realtek = 0x10ec,
        QEMUOld = 0x1234,
        VirtualBox = 0x80ee
    }; // enum VendorID

    enum DeviceID 
    {
        VirtIONetAdapter = 0x1000,
        VirtIOBlockDevice = 0x1001,
        VirtIOConsole = 0x1003,
        VirtIOEntropy = 0x1005,
        VirtIOGPU = 0x1050,
    }; // enum DeviceID

} // namespace Kernel::PCI
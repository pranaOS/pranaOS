#pragma once

namespace Kernel {

enum class PCIVendorID {
    VirtIO = 0x1af4,
    Intel = 0x8086,
};

enum class PCIDeviceID {
    VirtIOConsole = 0x1003,
    VirtIOEntropy = 0x1005,
};

}

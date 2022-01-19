//
// Created by KrisnaPranav on 19/01/22.
//

#pragma once

#include "manager.h"

namespace Kernel::Driver {
    struct pciDriverEntry {
        ak::uint16_t vendorID;
        ak::uint16_t deviceID;
        char* driverString;
    } __attribute__((packed));

    class pciDriver {
      public:
        static void assignDriversPCI(pci, driverManager* drivermanager);
    };
}
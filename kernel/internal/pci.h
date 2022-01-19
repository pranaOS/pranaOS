//
// Created by KrisnaPranav on 19/01/22.
//

#pragma once

#include <cpu/port.h>
#include <ak/list.h>
#include <system/console.h>
#include <system/init.h>
#include "systemcomponent.h"

namespace Kernel {
    #define PCI_CMDREG_IO (1 << 0)
    #define PCI_CMDREG_MEM (1 << 1)
    #define PCI_CMDREG_BM (1 << 2)

    struct pciDevice {
        ak::uint8_t interrupt;
        ak::uint8_t classID;
        ak::uint8_t subclassID;
        ak::uint8_t interfaceID;
        ak::uint8_t revisionID;

        ak::uint16_t bus;
        ak::uint16_t device;
        ak::uint16_t function;
        ak::uint16_t deviceID;
        ak::uint16_t vendorID;

        ak::uint32_t portBase;
    } __attribute__((packed));

    enum baseAddressRegister {
        MemoryMapping = 0,
        InputOutput = 1
    };

    struct baseAddressRegister {
        bool prefetchable;
        baseAddressRegister type;
        ak::uint32_t size;
        ak::uint32_t address;
    };

    class PCIController : public systemComponent {
      public:
        PCIController();
        List<pciDevice*> deviceList;

        void populateDeviceList();
    };
}
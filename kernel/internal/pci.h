#pragma once

#include <cpu/port.h>
#include <ak/list.h>
#include <system/console.h>
#include <system/init.h>
#include "systemcomponent.h"

namespace Kernel {
    #define PCI_CMDREG_IO   (1<<0)
    #define PCI_CMDREG_MEM  (1<<1)
    #define PCI_CMDREG_BM   (1<<2)

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

    struct baseAddress {
        bool prefetchable;
        baseAddressRegister type;
        ak::uint32_t size;
        ak::uint64_t address;
    };

    class PCIController : public systemComponent {
      public:
        ak::uint32_t read(ak::uint16_t bus, ak::uint16_t device, ak::uint16_t function, ak::uint32_t registeroffset);
        void write(ak::uint16_t bus, ak::uint16_t device, ak::uint16_t function, ak::uint32_t registeroffset, ak::uint32_t value);
        baseAddress getBaseAddressRegister(ak::uint16_t bus, ak::uint16_t device, ak::uint16_t function, ak::uint16_t bar);

      public:
        List<pciDevice*> deviceList;
        PCIController();
        void populateDeviceList();
    
      private:
        bool deviceHasFunctions(ak::uint16_t bus, ak::uint16_t device);
        char* getClassCodeString(ak::uint8_t classID, ak::uint8_t subClassID);
    };
}
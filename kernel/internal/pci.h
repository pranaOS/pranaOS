#pragma once

#include <cpu/port.h>
#include <ak/list.h>
#include <kernel/console.h>
#include <kernel/init.h>
#include "systemcomponent.h"

namespace Kernel {
    #define PCI_CMDREG_IO   (1<<0)
    #define PCI_CMDREG_MEM  (1<<1)
    #define PCI_CMDREG_BM   (1<<2)

    struct pciDevice {
        uint8_t interrupt;
        uint8_t classID;
        uint8_t subclassID;
        uint8_t interfaceID;
        uint8_t revisionID;            

        uint16_t bus;
        uint16_t device;
        uint16_t function;
        uint16_t deviceID;
        uint16_t vendorID;

        uint32_t portBase;
    } __attribute__((packed));

    enum baseAddressRegister {
        MemoryMapping = 0,
        InputOutput = 1
    };

    struct baseAddress {
        bool prefetchable;
        baseAddressRegister type;
        uint32_t size;
        uint64_t address;
    };

    class pciController : public systemComponent {
      public:
        uint32_t read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset);
        void write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value);
        baseAddress getBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar);

      public:
        List<pciDevice*> deviceList;
        pciController();
        void populateDeviceList();
    
      private:
        bool deviceHasFunctions(uint16_t bus, uint16_t device);
        char* getClassCodeString(uint8_t classID, uint8_t subClassID);
    };
}
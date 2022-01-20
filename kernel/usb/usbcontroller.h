//
// Created by KrisnaPranav on 20/01/22.
//

#pragma once

#include <kernel/drivers/driver.h>
#include <kernel/internal/pci.h>
#include <kernel/system/interrupthandler.h>
#include "usbdevice.h"

namespace Kernel {
    enum usbControllerType {
        UHCI,
        OHCI,
        EHCI,
        xHCI
    };

    class usbDriver;
    typedef struct interruptTransfer {
        uint8_t* bufferPointer;
        uint32_t bufferPhys;
        uint32_t tdPhys;

        usbDriver* handler;

        int bufferLen;
        int queueIndex;
        int endpoint;
        int numTd;

        void* qh;
        void* td
    };

    class usbDevice;

    class usbController {

    };
}
//
// Created by KrisnaPranav on 20/01/22.
//

#pragma once

#include <kernel/drivers/usb/defs.h>

namespace Kernel {
    enum endpointDirection : int {
        Out,
        In
    };

    enum endPointType : int {
        Control,
        Isochronous,
        Bulk,
        Interrupt
    };

    class usbEndpoint {
      public:
        ak::uint8_t endpointNumber = 0;
        
      private:
        bool currentState = false;
    };

}
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


    enum endpointType : int {
      Control,
      Isochronous,
      Bulk,
      Interrupt
    };

    class usbEndpoint {
      public:
        uint8_t endpointNumber = 0; 
        endpointDirection dir = endpointDirection::Out;
        endpointType type = endpointType::Control;
        uint16_t maxPacketSize = 0; 
        uint8_t interval = 0; 

      public:
        usbEndpoint(struct ENDPOINT_DESC* src);
        bool toggle();
        void setToggle(bool v);

      private:
        bool toggleState = false;
    };
}
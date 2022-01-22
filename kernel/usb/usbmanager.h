//
// Created by KrisnaPranav on 22/01/22.
//

#pragma once

#include "usbcontroller.h"
#include "usbdevice.h"

namespace Kernel {
    class usbManager {
      public:
        List<usbController*> controllerList;
        List<usbDevice*> deviceList;
        bool initDone = false;

        usbManager();

        void addController(usbController* c);
        void removeController(usbController* c);
        void addDevice(usbDevice* c);
        void removeDevice(usbController* controller, uint8_t port);
    };
}

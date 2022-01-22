//
// Created by KrisnaPranav on 22/01/22.
//

#pragma once

#include "usbcontroller.h"
#include "usbdevice.h"

namespace Kernel {
    /**
     * @brief: usb manager[add controller, remove controller, add device, remove device, setup all, assign all drivers, usb poll]
     */
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
        void setupAll();
        void assignAllDrivers();
        void usbPoll();

        bool initialize();

      private:
        usbManager();
        ~usbManager();
    };
}

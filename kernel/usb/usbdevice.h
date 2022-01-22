//
// Created by KrisnaPranav on 20/01/22.
//

#pragma once

#include "usbcontroller.h"
#include "usbendpoint.h"

namespace Kernel {
    class usbController;
    class usbDriver;

    class usbDevice {
    public:
        usbDevice();
        ~usbDevice();

        bool assignDriver();
        bool onUnplugged();
    
    public:
        uint8_t portNum = 0;
        char* deviceName = 0;
        usbController* controller = 0;

        usbDriver* driver = 0;
        
        struct uhciProperties {
            bool lowSpeed;
            int maxPacket;
        } uhciProperties_t;

        struct ohciProperties {
            int descMps;
            bool lsDevice;
        } ohciProperties_t;
    };
}
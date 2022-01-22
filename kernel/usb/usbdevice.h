//
// Created by KrisnaPranav on 20/01/22.
//

#pragma once

#include "usbcontroller.h"

namespace Kernel {
    class usbController;
    class usbDriver;

    class usbDevice {
    public:
        bool assignDriver();
        bool onUnplugged();
    };
}
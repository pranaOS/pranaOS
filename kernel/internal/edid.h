//
// Created by KrisnaPranav on 23/01/22.
//

#pragma once

#include "systemcomponent.h"

namespace Kernel {
    struct edidInfoBlock {
        ak::uint8_t header[8];
        ak::uint16_t manufacturerid;
    };

    #define EDID_SUCCESS_CODE 0x004F

    class edid : public systemComponent {
      public:
        edid();

        void acquireEDID();
        void preferedMode(int* widthPtr, int* heightPtr);
    };
}
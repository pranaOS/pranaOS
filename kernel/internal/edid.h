//
// Created by KrisnaPranav on 23/01/22.
//

#pragma once

#include "systemcomponent.h"

namespace Kernel {
    struct edidInfoBlock {
        ak::uint8_t header[8];
        ak::uint16_t manufacturerId;
        ak::uint16_t productId;
        ak::uint32_t serialNumber;
        ak::uint8_t weekOfManufacture
        ak::uint8_t yearOfManufacture;
        ak::uint8_t version;
        ak::uint8_t revision;
    };

    #define EDID_SUCCESS_CODE 0x004F

    class edid : public systemComponent {
      public:
        edid();

        void acquireEDID();
        void preferedMode(int* widthPtr, int* heightPtr);
    };
}
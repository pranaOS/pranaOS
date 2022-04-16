//
// Created by KrisnaPranav on 23/01/22.
//

#pragma once

#include "systemcomponent.h"

namespace Kernel {

    struct edidInfoBlock {
        uint8_t header[8];
        uint8_t weekOfManufacture;
        uint8_t yearOfManufacture;
        uint8_t version;
        uint8_t revision;
        uint8_t videoInputDefinition;
        uint8_t maxHorizontalImageSize;
        uint8_t maxVerticalImageSize;
        uint8_t displayGamma;
        uint8_t featureSupport;
        uint8_t redGreenLo;
        uint8_t blueWhiteLo;
        uint8_t redxhi;
        uint8_t redyhi;
        uint8_t greenxhi;
        uint8_t greenyhi;
        uint8_t bluexhi;
        uint8_t blueyhi;
        uint8_t whitexhi;
        uint8_t whiteyhi;

        uint8_t establishedTimings1;
        uint8_t establishedtimings2;
        uint8_t manufacturerReservedTimings;

        uint16_t standardTimings[8];
        uint16_t manufacturerOd;
        uint16_t productId;
        
        uint32_t serialNumber;

        struct detailTmings{
            uint16_t flag; 
            uint8_t flag2;
            uint8_t dataType;
            uint8_t flag3;
            uint8_t descriptorData[13];
        } detailtmings_t [4];

        uint8_t extensionFlag;
        uint8_t checksum;
    } __attribute__((packed));

    struct timingsInfoBlock {
        uint16_t pixelClock;
        uint8_t horizontalActiveLo;
        uint8_t horizontalBlankingLo;
        uint8_t horizontalHi;
        uint8_t verticalActiveLo;
        uint8_t verticalBlankingLo;
        uint8_t verticalHi;
        uint8_t horizontalSyncOffsetLo;
        uint8_t horizontalSyncPulsewidthLo;
        uint8_t verticalSyncLo;
        uint8_t syncHi;
        uint8_t horizontalImageSizeLo;
        uint8_t verticalImageSizeLo;
        uint8_t imageSizeHi;
        uint8_t horizontalBorder;
        uint8_t verticalBorder;
        uint8_t flags;
    } __attribute__((packed));

    #define EDID_SUCCESS_CODE 0x004F

    class edid : public systemComponent {
      public:
        edid();

        void acquireEDID();
        void preferedMode(int* widthPtr, int* heightPtr);
    };
}
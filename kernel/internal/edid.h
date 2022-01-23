//
// Created by KrisnaPranav on 23/01/22.
//

#pragma once

#include "systemcomponent.h"

namespace Kernel {

    struct edidInfoBlock {
        ak::uint8_t header[8];
        ak::uint8_t weekOfManufacture;
        ak::uint8_t yearOfManufacture;
        ak::uint8_t version;
        ak::uint8_t revision;
        ak::uint8_t videoInputDefinition;
        ak::uint8_t maxHorizontalImageSize;
        ak::uint8_t maxVerticalImageSize;
        ak::uint8_t displayGamma;
        ak::uint8_t featureSupport;
        ak::uint8_t redGreenLo;
        ak::uint8_t blueWhiteLo;
        ak::uint8_t redxhi;
        ak::uint8_t redyhi;
        ak::uint8_t greenxhi;
        ak::uint8_t greenyhi;
        ak::uint8_t bluexhi;
        ak::uint8_t blueyhi;
        ak::uint8_t whitexhi;
        ak::uint8_t whiteyhi;

        ak::uint8_t establishedTimings1;
        ak::uint8_t establishedtimings2;
        ak::uint8_t manufacturerReservedTimings;

        ak::uint16_t standardTimings[8];
        ak::uint16_t manufacturerOd;
        ak::uint16_t productId;
        
        ak::uint32_t serialNumber;

        struct detailTmings{
            ak::uint16_t flag; 
            ak::uint8_t flag2;
            ak::uint8_t dataType;
            ak::uint8_t flag3;
            ak::uint8_t descriptorData[13];
        } detailtmings_t [4];

        ak::uint8_t extensionFlag;
        ak::uint8_t checksum;
    } __attribute__((packed));

    struct timingsInfoBlock {
        ak::uint16_t pixelClock;
        ak::uint8_t horizontalActiveLo;
        ak::uint8_t horizontalBlankingLo;
        ak::uint8_t horizontalHi;
        ak::uint8_t verticalActiveLo;
        ak::uint8_t verticalBlankingLo;
        ak::uint8_t verticalHi;
        ak::uint8_t horizontalSyncOffsetLo;
        ak::uint8_t horizontalSyncPulsewidthLo;
        ak::uint8_t verticalSyncLo;
        ak::uint8_t syncHi;
        ak::uint8_t horizontalImageSizeLo;
        ak::uint8_t verticalImageSizeLo;
        ak::uint8_t imageSizeHi;
        ak::uint8_t horizontalBorder;
        ak::uint8_t verticalBorder;
        ak::uint8_t flags;
    } __attribute__((packed));

    #define EDID_SUCCESS_CODE 0x004F

    class edid : public systemComponent {
      public:
        edid();

        void acquireEDID();
        void preferedMode(int* widthPtr, int* heightPtr);
    };
}
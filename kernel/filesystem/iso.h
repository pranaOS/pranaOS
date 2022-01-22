//
// Created by KrisnaPranav on 22/01/22.
//

#pragma once

#include <ak/types.h>
#include "virtualfilesystem.h"

namespace Kernel {
    struct directoryRecord {
        ak::uint8_t length;
        ak::uint8_t  earLength;
        ak::uint8_t extentLocation;
        ak::uint8_t extentLocationBe;
        ak::uint8_t dataLength;
        ak::uint8_t dataLengthBe;
        ak::uint8_t datetime[7];
        ak::uint8_t flags;
        ak::uint8_t gapSize;
        ak::uint8_t unitSize;
        ak::uint8_t volSeqNumber;
        ak::uint8_t volSeqNumberBe;
        ak::uint8_t nameLength;
        char name[222];
    } __attribute__((packed));

    struct timeFormat {
        char year[4];
        char month[2];
        char day[2];
        char hour[2];
        char minute[2];
        char second[2];
        cahr hundrdSecond[2];
        ak::int8_t timeZon;
    } __attribute__((packed));

    struct volumeDescriptor {
        ak::uint8_t type;
        char identifier[5];
        char data[2];
    };
}

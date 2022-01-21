//
// Created by KrisnaPranav on 21/01/22.
//

#pragma once

#include "virtualfilesystem.h

namespace Kernel {
    struct Fat32 {
        ak::uint8_t bootCode[3];
        ak::uint8_t oemId[8];
        ak::uint16_t bytesPerSector;
        ak::uint8_t sectorsPerCluster;
        ak::uint16_t reverseSectors;
        ak::uint8_t numOfFats;
        ak::uint16_t numDirEntries;
        ak::uint16_t totalSectorsSmall;
        ak::uint8_t mediaDescriptionType;
    };

    struct Fat32_Info {

    };

    struct directoryEntry {

    };

    struct lfnEntry {

    };

    struct fatEntryInfo {

    };
}
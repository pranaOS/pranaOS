#pragma once

#include <ak/types.h>
#include <ak/convert.h>
#include <ak/string.h>
#include <ak/memoperator.h>
#include <ak/list.h>
#include <kernel/console.h>
#include "disk.h"

namespace Kernel {
    struct biosDriveParameters {
        uint8_t  interfacePath[8];
        uint8_t  devicePath[8];
        uint8_t  reserved2;
        uint8_t  checksum;
        uint8_t  devPathLen;
        uint8_t  reserved1[3];
        uint16_t bufLen;
        uint16_t bytesPerSector;
        uint16_t infoFlags;
        uint16_t signature;
        uint32_t cilinders;
        uint32_t heads;
        uint32_t sectorsPerTrack;
        uint32_t eddParameters;
        uint64_t totalSectors;
        char             hostBusName[4];
        char             interfaceName[8];
    } __attribute__((packed));

    class Disk;

    class diskManager {
    public:
        List<Disk*> allDisks;
        diskManager();

        void addDisk(Disk* disk);
        void removeDisk(Disk* disk);

        char readSector(uint16_t drive, uint32_t lba, uint8_t* buf);
        char writeSector(uint16_t drive, uint32_t lba, uint8_t* buf);

        biosDriveParameters* getDriveInfoBios(uint8_t drive);
    };
}
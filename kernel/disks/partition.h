//
// Created by KrisnaPranav on 21/01/22.
//

#pragma once

#include "diskmanager.h"
#include <kernel/filesystem/vfsmanager.h>

namespace Kernel {

    struct partitionTableEntry {
        uint8_t bootable;

        uint8_t startHead;
        uint8_t startSector : 6;
        uint16_t startCylinder : 10;

        uint8_t partitionId;

        uint8_t endHead;
        uint8_t endSector : 6;
        uint16_t endCylinder : 10;

        uint32_t startLba;
        uint32_t length;
    } __attribute__((packed));

    struct masterBootRecord {
        uint8_t bootloader[440];
        uint32_t signature;
        uint16_t unused;
            
        partitionTableEntry primaryPartitions[4];
            
        uint16_t magicnumber;
    } __attribute__((packed));

    class partitionManager {
      public:
        static void detectFileSystem(Disk* disk);

      private:
        static void assignVFS(partitionTableEntry partition, Disk* disk);
    };
}
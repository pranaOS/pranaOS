#pragma once

#include <ak/types.h>
#include "diskcontroller.h"

namespace Kernel {
    
    class diskController;

    enum diskType {
        hardDisk,
        usbDisk,
        floppy,
        cdROM
    };

    class Disk {
    public:
        diskController* controller;
        uint32_t controllerIndex;
        char* identifier = 0;
        diskType type;
        uint64_t size;
        uint32_t numBlocks;
        uint32_t blockSize;

        Disk(uint32_t controllerIndex, diskController* controller, diskType type, uint64_t size, uint32_t blocks, uint32_t blocksize);
            
        virtual char readSector(uint32_t lba, uint8_t* buf);
        virtual char writeSector(uint32_t lba, uint8_t* buf);
    };
    
}
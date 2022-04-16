#pragma once

#include <ak/types.h>
#include <ak/convert.h>
#include <ak/memoperator.h>
#include "disk.h"
#include "diskmanager.h"

namespace Kernel {
    class diskController {
    public:
        diskController();

        char readSector(uint16_t drive, uint32_t lba, uint8_t* buf);
        char writeSector();
        bool ejectDrive(uint8_t drive);
    };
}
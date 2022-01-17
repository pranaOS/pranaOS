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

        char readSector(ak::uint16_t drive, ak::uint32_t lba, ak::uint8_t* buf);
        char writeSector();
        bool ejectDrive(ak::uint8_t drive);
    };
}
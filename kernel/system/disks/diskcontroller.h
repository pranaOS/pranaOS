//
//  diskcontroller.h
//  pranaOS
//
//  Created by Krisna Pranav on 14/01/22.
//

#pragma once

#include <ak/types.h>
#include <ak/convert.h>
#include <ak/memoperator.h>
#include <system/disks/disk.h>
#include <system/disks/diskmanager.h>

namespace Kernel {
    namespace system {
        class diskController {
        public:
            diskController();

            char readSector(ak::uint16_t drive, ak::uint32_t lba, ak::uint8_t* buf);
            char writeSector();
            bool ejectDrive(ak::uint8_t drive);
        };
    }
}
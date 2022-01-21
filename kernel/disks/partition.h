//
// Created by KrisnaPranav on 21/01/22.
//

#pragma once

#include "diskmanager.h"
#include <kernel/filesystem/vfsmanager.h>

namespace Kernel {
    struct PartitionTableEntry {
        ak::uint8_t bootable;

        ak::uint8_t startHead;
        ak::uint8_t startSector : 6;
        ak::uint16_t startCylinder : 10;

        ak::uint8_t partitionId;

        ak::uint8_t endHead;
        ak::uint8_t endSector : 6;
        ak::uint16_t endCylinder : 10;

        ak::uint32_t startLba;
        ak::uint32_t length;
    } __attribute__((packed));

    struct masterBootRecord {

    } __attribute__((packed));
}
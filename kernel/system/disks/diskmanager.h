//
//  diskmanager.h
//  pranaOS
//
//  Created by Krisna Pranav on 14/01/22.
//

#pragma once

#include <ak/types.h>
#include <ak/convert.h>
#include <ak/string.h>
#include <ak/memoperator.h>
#include <ak/list.h>
#include <system/console.h>
#include <system/disks/disk.h>

namespace Kernel {
    namespace system {
        struct biosDriveParameters {
            ak::uint16_t bufLen;
            ak::uint16_t infoFlags;
            ak::uint32_t cilinders;
            ak::uint32_t heads;
            ak::uint32_t sectorsPerTrack;
            ak::uint64_t totalSectors;
            ak::uint16_t bytesPerSector;

            ak::uint32_t eddParameters;

            ak::uint16_t signature;
            ak::uint8_t  devPathLen;
            ak::uint8_t  reserved1[3];
            char             hostBusName[4];
            char             interfaceName[8];
            ak::uint8_t  interfacePath[8];
            ak::uint8_t  devicePath[8];
            ak::uint8_t  reserved2;
            ak::uint8_t  checksum;
        } __attribute__((packed));
    }
}

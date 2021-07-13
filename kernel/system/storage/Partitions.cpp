/*
 * Copyright (c) 2021, evilbat831
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/Streams.h"
#include "mbr/MBR.h"
#include "system/devices/Devices.h"
#include "system/storage/Partition.h"
#include "system/storage/Partitions.h"

bool partition_load_mbr(RefPtr<Device> disk, const MBR &mbr)
{
    if (mbr.magic != 0xAA55)
    {
        return false;
    }

    Kernel::logln("MBR on '{}': ", disk->path().cstring());
    Kernel::logln("    - magic     = {#04x}", mbr.magic);
    Kernel::logln("    - signature = {#08x}", mbr.signature);

    for (size_t i = 0; i < 4; i++)
    {
        const MBREntry &entry = mbr.entries[i];

        Kernel::logln("    - Partition[{}] = {start={8d}, size={8d}, type=0x{x}}", i, entry.start, entry.size, entry.type);

        if (entry.type != 0)
        {
            disk->add(make<Partition>(disk, i, entry.start * 512, entry.size * 512));
        }
    }

    return true;
}

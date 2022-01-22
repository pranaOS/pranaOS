//
// Created by KrisnaPranav on 22/01/22.
//

#pragma once

#include <ak/types.h>
#include "virtualfilesystem.h"

namespace Kernel {
    struct directoryRecord {
        ak::uint8_t length;
        ak::uint8_t  earLength;
        ak::uint8_t extentLocation;
        ak::uint8_t extentLocationBe;
        ak::uint8_t dataLength;
        ak::uint8_t dataLengthBe;
        ak::uint8_t datetime[7];
        ak::uint8_t flags;
        ak::uint8_t gapSize;
        ak::uint8_t unitSize;
        ak::uint8_t volSeqNumber;
        ak::uint8_t volSeqNumberBe;
        ak::uint8_t nameLength;
        char name[222];
    } __attribute__((packed));

    struct timeFormat {
        char year[4];
        char month[2];
        char day[2];
        char hour[2];
        char minute[2];
        char second[2];
        cahr hundrdSecond[2];
        ak::int8_t timeZon;
    } __attribute__((packed));

    struct volumeDescriptor {
        ak::uint8_t type;
        char identifier[5];
        ak::uint8_t version;
        char data[2041];
    };

    #define ISODCL(from, to) (to - from + 1)

    struct primaryVolumeDescriptor {
        ak::uint8_t type;
        char id                             [ISODCL (  2,     6)];
        ak::uint8_t version;
        char reserved1                      [ISODCL (  8,     8)];
        char system_id                      [ISODCL (  9,    40)]; 
        char volume_id                      [ISODCL ( 41,    72)];
        char reserved2                      [ISODCL ( 73,    80)];
        ak::uint64_t volume_space_size;
        char reserved3                      [ISODCL ( 89,   120)];
        ak::uint32_t volume_set_size;
        ak::uint32_t volume_sequence_number;
    };

    #define ISO_START_SECTOR 0x10
    #define CDROM_SECTOR_SIZE 2048

    enum volumeDescriptorType {
        bootRecord = 0,
        pvDescriptor = 1,
        supplementaryVolumeDescriptor = 2,
        volumePartitionDescriptor = 3,
        volumeDescriptorSetTerminator = 255
    };

    enum isoEntryType {
        isoFile = 0,
        isoDirectory = 1
    };
}

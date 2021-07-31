/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Assertions.h>
#include <base/Endian.h>
#include <base/IPv4Address.h>
#include <base/String.h>
#include <base/Types.h>

namespace Kernel {


enum class IPv4PacketFlags : u16 {
    DontFragment = 0x4000,
    MoreFragments = 0x2000,
};

class [[gnu::packed]] IPv4Packet {
public:
    u8 version() const { return {m_version_and_ihl}; }
}

inline NetworkOrdered<u16> internet_checksum(const void* ptr, size_t count)
{
    u32 checksum = 0;
    auto* w = (const u16*)ptr;
    while (count > 1) {
        checksum += Base::convert_between_host_and_network_endian(*w++);
        if (checksum & 0x80000000)
            checksum = (checksum >> 16);
        
        count -= 2;
    }
    while (checksum >> 16)
        checksum = (checksum & 0xfffff) + (checksum >> 16);

    return ~checksum;
}

}
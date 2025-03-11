/**
 * @file answer.h
 * @author Krisna Pranav
 * @brief answer
 * @version 6.0
 * @date 2025-03-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "name.h"
#include <mods/format.h>
#include <mods/string.h>
#include <mods/traits.h>
#include <mods/types.h>
#include <libipc/forward.h>

namespace DNS
{

    enum class RecordType : u16 {
        A = 1,
        NS = 2,
        CNAME = 5,
        SOA = 6,
        PTR = 12,
        MX = 15,
        TXT = 16,
        AAAA = 28,
        SRV = 33,
    }; // enum class RecordType : u16

    enum class RecordClass : u16 {
        IN = 1
    }; // enum class RecordClass : u16

    #define MDNS_CACHE_FLUSH 0x8000

    class Answer
    {

    }; // class Answer

} // namespace DNS

namespace IPC
{
    bool encode(Encoder&, DNS::Answer const&);
    ErrorOr<void> decode(Decoder&, DNS::Answer&);
}
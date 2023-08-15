/**
 * @file network_adapter.h
 * @author Krisna Pranav aka (krishpranav)
 * @brief network adapter
 * @version 1.0
 * @date 2023-08-15
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/userorkernelbuffer.h>
#include <kernel/kbuffer.h>
#include <kernel/net/ipv4.h>
#include <kernel/net/icmp.h>
#include <kernel/net/ipv4.h>
#include <mods/byte_buffer.h>
#include <mods/function.h>
#include <mods/mac_address.h>
#include <mods/singlelinkedlist.h>
#include <mods/types.h>
#include <mods/weakable.h>
#include <mods/weakptr.h>

namespace Kernel
{
    class NetworkAdapter;

    class NetworkAdapter : public RefCounted<NetworkAdapter>
    {
        
    }; // class NetworkAdapter
    
} // namespace Kernel
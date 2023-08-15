/**
 * @file socket.h
 * @author Krisna Pranav
 * @brief socket
 * @version 1.0
 * @date 2023-08-15
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/hashtable.h>
#include <mods/nonnullrefptrvector.h>
#include <mods/refcounted.h>
#include <kernel/lock.h>
#include <kernel/kresult.h>
#include <kernel/unixtypes.h>
#include <kernel/filesystem/file.h>
#include <kernel/net/network_adapter.h>

namespace Kernel
{

    enum class ShouldBlock
    {
        No = 0,
        Yes = 1,
    }; // enum

    class FileDescription;

    

} // namespace Kernel
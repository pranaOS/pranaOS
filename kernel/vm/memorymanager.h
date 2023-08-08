/**
 * @file memorymanager.h
 * @author Krisna Pranav
 * @brief memory_manager
 * @version 1.0
 * @date 2023-08-08
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/forward.h>
#include <kernel/spinlock.h>
#include <kernel/arch/i386/cpu.h>
#include <kernel/vm/physicalpage.h>
#include <kernel/vm/region.h>
#include <kernel/vm/vmobject.h>
#include <mods/hashtable.h>
#include <mods/nonnullrefptrvector.h>
#include <mods/string.h>

namespace Kernel
{

} // namespace Kernel
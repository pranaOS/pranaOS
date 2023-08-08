/**
 * @file region.h
 * @author Krisna Pranav
 * @brief region
 * @version 1.0
 * @date 2023-08-08
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once
 
#include <kernel/arch/i386/cpu.h>
#include <kernel/heap/slaballocator.h>
#include <kernel/vm/rangeallocator.h>
#include <kernel/vm/vmobject.h>
#include <mods/inlinelinkedlist.h>
#include <mods/string.h>
#include <mods/weakable.h>

namespace Kernel
{
    class Inode;
    class VMObject;

    enum class PageFaultResponse
    {
        ShouldCrash,
        OutOfMemory,
        Continue,
    }; 

    class Region final
        : public InlineLinkedListNode<Region>
        , public Weakable<Region> {
            
        friend class MemoryManager;
    }
} // namespace Kernel   
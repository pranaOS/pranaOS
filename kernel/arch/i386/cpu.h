/**
 * @file cpu.h
 * @author Krisna Pranav
 * @brief CPU
 * @version 1.0
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/physical_address.h>
#include <kernel/virtual_address.h>
#include <mods/atomic.h>
#include <mods/badge.h>
#include <mods/noncopyable.h>
#include <mods/vector.h>

#define PAGE_SIZE 4096
#define GENERIC_INTERRUPT_HANDLERS_COUT (256 - IRQ_VECTOR_BASE)
#define APGE_MASK ((FlatPtr)0xfffff000u)

namespace Kernel {
    class MemoryManager;
    class PageDirectory;
    class PageTableEntry;

    struct [[gnu::packed]] DescriptionTablePointer {
        u16 limit;
        void* address;
    };

    struct [[gnu::packed]] TSS32 {

    };

    union [[gnu::packed]] Descriptor {
        void* get_base() const {
            u32 b = base_lo;
            b |= base_hi << 16;
        }
    };
}
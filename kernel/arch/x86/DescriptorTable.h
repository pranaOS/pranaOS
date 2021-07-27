/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Types.h>
#include <kernel/VirtualAddress.h>

#if ARCH(I386)
#    define GDT_SELECTOR_CODE0 0x08
#    define GDT_SELECTOR_DATA0 0x10
#    define GDT_SELECTOR_CODE3 0x18
#    define GDT_SELECTOR_DATA3 0x20
#    define GDT_SELECTOR_TLS 0x28
#    define GDT_SELECTOR_PROC 0x30
#    define GDT_SELECTOR_TSS 0x38

static_assert(GDT_SELECTOR_CODE0 + 8 == GDT_SELECTOR_DATA0); 

static_assert(GDT_SELECTOR_CODE0 + 16 == GDT_SELECTOR_CODE3); 
static_assert(GDT_SELECTOR_CODE0 + 24 == GDT_SELECTOR_DATA3); 
#else
#    define GDT_SELECTOR_CODE0 0x08
#    define GDT_SELECTOR_CODE3 0x10
#    define GDT_SELECTOR_DATA3 0x18
#    define GDT_SELECTOR_TSS 0x20
#    define GDT_SELECTOR_TSS_PART2 0x28
#endif

namespace Kernel {

struct [[gnu::packed]] DescriptorTablePointer {
    u16 limit;
    void* address;
};

union [[gnu::packed]] Descriptor {
    struct {
        u16 limit_lo;
        u16 base_lo;
        u8 base_hi;
        u8 type : 4;
        u8 descriptor_type : 1;
        u8 dpl : 2;
        u8 segment_present : 1;
        u8 limit_hi : 4;
        u8 : 1;
        u8 operation_size64 : 1;
        u8 operation_size32 : 1;
        u8 granularity : 1;
        u8 base_hi2;
    };
    struct {
        u32 low;
        u32 high;
    };

    enum Type {
        Invalid = 0,
        AvailableTSS_16bit = 0x1,
        LDT = 0x2,
        BusyTSS_16bit = 0x3,
        CallGate_16bit = 0x4,
        TaskGate = 0x5,
        InterruptGate_16bit = 0x6,
        TrapGate_16bit = 0x7,
        AvailableTSS_32bit = 0x9,
        BusyTSS_32bit = 0xb,
        CallGate_32bit = 0xc,
        InterruptGate_32bit = 0xe,
        TrapGate_32bit = 0xf,
    };

    VirtualAddress base() const
    {
        FlatPtr base = base_lo;
        base |= base_hi << 16u;
        base |= base_hi2 << 24u;
        return VirtualAddress { base };
    }

    void set_base(VirtualAddress base)
    {
        base_lo = base.get() & 0xffffu;
        base_hi = (base.get() >> 16u) & 0xffu;
        base_hi2 = (base.get() >> 24u) & 0xffu;
        VERIFY(base.get() <= 0xffffffff);
    }

    void set_limit(u32 length)
    {
        limit_lo = length & 0xffff;
        limit_hi = (length >> 16) & 0xf;
    }
};

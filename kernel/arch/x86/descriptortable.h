/**
 * @file descriptortable.h
 * @author Krisna Pranav
 * @brief descriptor table
 * @version 6.0
 * @date 2024-08-30
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/stdlibextra.h>
#include <mods/types.h>
#include <mods/platform.h>
#include <kernel/virtual_address.h>

VALIDATE_IS_X86()

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
#    define GDT_SELECTOR_DATA0 0x10
#    define GDT_SELECTOR_DATA3 0x18
#    define GDT_SELECTOR_CODE3 0x20
#    define GDT_SELECTOR_TSS 0x28
#    define GDT_SELECTOR_TSS_PART2 0x30
#endif

namespace Kernel 
{
    struct [[gnu::packed]] DescriptorTablePointer 
    {
        u16 limit;
        void* address;
    }; // struct [[gnu::packed]] DescriptorTablePointer

    union [[gnu::packed]] Descriptor 
    {
        struct 
        {
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

        struct 
        {
            u32 low;
            u32 high;
        };

        enum SystemType 
        {
            Invalid = 0,
            AvailableTSS_16bit = 0x1,
            LDT = 0x2,
            BusyTSS_16bit = 0x3,
            CallGate_16bit = 0x4,
            TaskGate = 0x5,
            InterruptGate_16bit = 0x6,
            TrapGate_16bit = 0x7,
            AvailableTSS = 0x9,
            BusyTSS = 0xb,
            CallGate = 0xc,
            InterruptGate = 0xe,
            TrapGate = 0xf,
        };

        VirtualAddress base() const
        {
            FlatPtr base = base_lo;
            base |= base_hi << 16u;
            base |= base_hi2 << 24u;
            return VirtualAddress { base };
        }

        /**
         * @param base 
         */
        void set_base(VirtualAddress base)
        {
            base_lo = base.get() & 0xffffu;
            base_hi = (base.get() >> 16u) & 0xffu;
            base_hi2 = (base.get() >> 24u) & 0xffu;
            VERIFY(base.get() <= 0xffffffff);
        }

        /**
         * @brief Set the_limit object
         * 
         * @param length 
         */
        void set_limit(u32 length)
        {
            limit_lo = length & 0xffff;
            limit_hi = (length >> 16) & 0xf;
        }
    }; // union [[gnu::packed]] Descriptor

    static_assert(AssertSize<Descriptor, 8>());

    enum class IDTEntryType 
    {
        TaskGate32 = 0b0101,
        InterruptGate16 = 0b110,
        TrapGate16 = 0b111,
        InterruptGate32 = 0b1110,
        TrapGate32 = 0b1111,
    }; // enum class IDTEntryType

    struct [[gnu::packed]] IDTEntry
    {
        u16 offset_1; 
        u16 selector; 

    #if ARCH(I386)
        u8 zero; 
    #else
        struct 
        {
            u8 interrupt_stack_table : 3;
            u8 zero : 5; 
        };
    #endif
        struct 
        {
            u8 gate_type : 4;
            u8 storage_segment : 1;
            u8 descriptor_privilege_level : 2;
            u8 present : 1;
        } type_attr;  

        u16 offset_2; 
    #if !ARCH(I386)
        u32 offset_3;
        u32 zeros;
    #endif

        IDTEntry() = default;

        /**
         * @param callback 
         * @param selector_ 
         * @param type 
         * @param storage_segment 
         * @param privilege_level 
         */
        IDTEntry(FlatPtr callback, u16 selector_, IDTEntryType type, u8 storage_segment, u8 privilege_level)
            : offset_1 { (u16)((FlatPtr)callback & 0xFFFF) }
            , selector { selector_ }
    #if !ARCH(I386)
            , interrupt_stack_table { 0 }
    #endif
            , zero { 0 }
            , type_attr {
                .gate_type = (u8)type,
                .storage_segment = storage_segment,
                .descriptor_privilege_level = (u8)(privilege_level & 0b11),
                .present = 1,
            }
            , offset_2 { (u16)((FlatPtr)callback >> 16) }
    #if !ARCH(I386)
            , offset_3 { (u32)(((FlatPtr)callback) >> 32) }
            , zeros { 0 }
    #endif
        {
        }
        
        /**
         * @return FlatPtr 
         */
        FlatPtr off() const
        {
    #if ARCH(I386)
            return (u32)offset_2 << 16 & (u32)offset_1;
    #else
            return (u64)offset_3 << 32 & (u64)offset_2 << 16 & (u64)offset_1;
    #endif
        }

        /**
         * @return IDTEntryType 
         */
        IDTEntryType type() const
        {
            return IDTEntryType(type_attr.gate_type);
        }
    }; // struct [[gnu::packed]] IDTEntry


    static_assert(AssertSize<IDTEntry, 2 * sizeof(void*)>());
} // namespace Kernel

/**
 * @file cpu.h
 * @author Krisna Pranav
 * @brief CPU
 * @version 6.0
 * @date 2024-09-13
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/atomic.h>
#include <mods/concept.h>
#include <mods/vector.h>
#include <mods/platform.h>
#include <kernel/arch/x86/descriptortable.h>

VALIDATE_IS_X86()

#define IRQ_VECTOR_BASE 0x50
#define GENERIC_INTERRUPT_HANDLERS_COUNT (256 - IRQ_VECTOR_BASE)
#define PAGE_MASK (~(FlatPtr)0xfffu)

namespace Kernel 
{

    struct RegisterState;
    class GenericInterruptHandler;

    static constexpr u32 safe_eflags_mask = 0xdff;
    static constexpr u32 iopl_mask = 3u << 12;

    /**
     * @brief Get the iopl from eflags object
     * 
     * @param eflags 
     * @return u32 
     */
    inline u32 get_iopl_from_eflags(u32 eflags)
    {
        return (eflags & iopl_mask) >> 12;
    }

    /**
     * @brief Get the gdtr object
     * 
     * @return const DescriptorTablePointer& 
     */
    const DescriptorTablePointer& get_gdtr();

    /**
     * @brief Get the idtr object
     * 
     * @return const DescriptorTablePointer& 
     */
    const DescriptorTablePointer& get_idtr();

    /**
     * @param description 
     * @param signal 
     * @param out_of_memory 
     */
    void handle_crash(RegisterState const&, char const* description, int signal, bool out_of_memory = false);

    #define LSW(x) ((u32)(x)&0xFFFF)
    #define MSW(x) (((u32)(x) >> 16) & 0xFFFF)
    #define LSB(x) ((x)&0xFF)
    #define MSB(x) (((x) >> 8) & 0xFF)

    /**
     * @param address 
     * @return constexpr FlatPtr 
     */
    constexpr FlatPtr page_base_of(FlatPtr address)
    {
        return address & PAGE_MASK;
    }

    /**
     * @param address 
     * @return FlatPtr 
     */
    inline FlatPtr page_base_of(const void* address)
    {
        return page_base_of((FlatPtr)address);
    }

    /**
     * @param address 
     * @return constexpr FlatPtr 
     */
    constexpr FlatPtr offset_in_page(FlatPtr address)
    {
        return address & (~PAGE_MASK);
    }

    /**
     * @param address 
     * @return FlatPtr 
     */
    inline FlatPtr offset_in_page(const void* address)
    {
        return offset_in_page((FlatPtr)address);
    }

} // namespace Kernel
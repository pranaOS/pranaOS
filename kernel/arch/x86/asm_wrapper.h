/**
 * @file asm_wrapper.h
 * @author Krisna Pranav
 * @brief asm wrapper
 * @version 6.0
 * @date 2024-08-25
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>
#include <mods/platform.h>

VALIDATE_IS_X86()

namespace Kernel 
{

    ALWAYS_INLINE void cli()
    {
        asm volatile("cli" ::
                        : "memory");
    }

    ALWAYS_INLINE void sti()
    {
        asm volatile("sti" ::
                        : "memory");
    }

    /**
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE FlatPtr cpu_flags()
    {
        FlatPtr flags;
        asm volatile(
            "pushf\n"
            "pop %0\n"
            : "=rm"(flags)::"memory");
        return flags;
    }

    #if ARCH(I386)
    /**
     * @brief Set the fs object
     * 
     * @param segment 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE void set_fs(u16 segment)
    {
        asm volatile(
            "mov %%ax, %%fs" ::"a"(segment)
            : "memory");
    }

    /**
     * @brief Set the gs object
     * 
     * @param segment 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE void set_gs(u16 segment)
    {
        asm volatile(
            "mov %%ax, %%gs" ::"a"(segment)
            : "memory");
    }

    /**
     * @brief Get the fs object
     * 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE u16 get_fs()
    {
        u16 fs;
        asm("mov %%fs, %%eax"
            : "=a"(fs));
        return fs;
    }

    /**
     * @brief Get the gs object
     * 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE u16 get_gs()
    {
        u16 gs;
        asm("mov %%gs, %%eax"
            : "=a"(gs));
        return gs;
    }
    #endif

    /**
     * @tparam T 
     * @param offset 
     * @return ALWAYS_INLINE 
     */
    template<typename T>
    ALWAYS_INLINE T read_gs_value(FlatPtr offset)
    {
        T val;
        asm volatile(
            "mov %%gs:%a[off], %[val]"
            : [val] "=r"(val)
            : [off] "ir"(offset));
        return val;
    }

    /**
     * @tparam T 
     * @param offset 
     * @param val 
     * @return ALWAYS_INLINE 
     */
    template<typename T>
    ALWAYS_INLINE void write_gs_value(FlatPtr offset, T val)
    {
        asm volatile(
            "mov %[val], %%gs:%a[off]" ::[off] "ir"(offset), [val] "r"(val)
            : "memory");
    }

    /**
     * @param offset 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE FlatPtr read_gs_ptr(FlatPtr offset)
    {
        FlatPtr val;
        asm volatile(
            "mov %%gs:%a[off], %[val]"
            : [val] "=r"(val)
            : [off] "ir"(offset));
        return val;
    }

    /**
     * @param offset 
     * @param val 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE void write_gs_ptr(u32 offset, FlatPtr val)
    {
        asm volatile(
            "mov %[val], %%gs:%a[off]" ::[off] "ir"(offset), [val] "r"(val)
            : "memory");
    }

    /**
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE bool are_interrupts_enabled()
    {
        return (cpu_flags() & 0x200) != 0;
    }

    FlatPtr read_cr0();
    FlatPtr read_cr2();
    FlatPtr read_cr3();
    FlatPtr read_cr4();

    u64 read_xcr0();

    void write_cr0(FlatPtr);
    void write_cr3(FlatPtr);
    void write_cr4(FlatPtr);

    void write_xcr0(u64);

    void flush_idt();

    /**
     * @param selector 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE void load_task_register(u16 selector)
    {
        asm("ltr %0" ::"r"(selector));
    }

    FlatPtr read_dr0();
    void write_dr0(FlatPtr);

    FlatPtr read_dr1();
    void write_dr1(FlatPtr);

    FlatPtr read_dr2();
    void write_dr2(FlatPtr);
    
    FlatPtr read_dr3();
    void write_dr3(FlatPtr);

    FlatPtr read_dr6();
    void write_dr6(FlatPtr);
    
    FlatPtr read_dr7();
    void write_dr7(FlatPtr);

    /**
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static bool is_kernel_mode()
    {
        u16 cs;
        asm volatile(
            "mov %%cs, %[cs] \n"
            : [cs] "=g"(cs));
        return (cs & 3) == 0;
    }

    /**
     * @param lsw 
     * @param msw 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE void read_tsc(u32& lsw, u32& msw)
    {
        asm volatile("rdtsc"
                    : "=d"(msw), "=a"(lsw));
    }

    /**
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE u64 read_tsc()
    {
        u32 lsw;
        u32 msw;
        read_tsc(lsw, msw);
        return ((u64)msw << 32) | lsw;
    }

    void stac();
    void clac();

    /**
     * @return ALWAYS_INLINE 
     */
    [[noreturn]] ALWAYS_INLINE void halt_this()
    {
        for (;;) {
            asm volatile("cli; hlt");
        }
    }
} // namespace Kernel
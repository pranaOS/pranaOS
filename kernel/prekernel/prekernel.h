/**
 * @file prekernel.h
 * @author Krisna Pranav
 * @brief Pre Kernel
 * @version 6.0
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#ifdef __cplusplus
#    include <kernel/multiboot.h>
#    include <kernel/physical_address.h>
#    include <kernel/virtual_address.h>
#endif

#define MAX_KERNEL_SIZE 0x4000000

#ifdef __cplusplus
namespace Kernel 
{
    struct [[gnu::packed]] BootInfo 
    {
        u32 start_of_prekernel_image;
        u32 end_of_prekernel_image;
        u64 physical_to_virtual_offset;
        u64 kernel_mapping_base;
        u64 kernel_load_base;
    #    if ARCH(X86_64)
        u32 gdt64ptr;
        u16 code64_sel;
        u32 boot_pml4t;
    #    endif
        u32 boot_pdpt;
        u32 boot_pd0;
        u32 boot_pd_kernel;
        u64 boot_pd_kernel_pt1023;
        u64 kernel_cmdline;
        u32 multiboot_flags;
        u64 multiboot_memory_map;
        u32 multiboot_memory_map_count;
        u64 multiboot_modules;
        u32 multiboot_modules_count;
        u64 multiboot_framebuffer_addr;
        u32 multiboot_framebuffer_pitch;
        u32 multiboot_framebuffer_width;
        u32 multiboot_framebuffer_height;
        u8 multiboot_framebuffer_bpp;
        u8 multiboot_framebuffer_type;
    }; // struct [[gnu::packed]] BootInfo 
} // namespace Kernel
#endif

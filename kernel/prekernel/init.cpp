/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Types.h>
#include <kernel/Multiboot.h>
#include <kernel/PhysicalAddress.h>
#include <kernel/Prekernel/Prekernel.h>
#include <kernel/VirtualAddress.h>
#include <libc/elf.h>
#include <libelf/Relocation.h>

extern size_t __stack_chk_guard;
size_t __stack_chk_guard;
extern "C" [[noreturn]] void __stack_chk_fail();

extern "C" u8 start_of_prekernel_image[];
extern "C" u8 end_of_prekernel_image[];

extern "C" u8 gdt64ptr[];
extern "C" u16 code64_sel;
extern "C" u64 boot_pml4t[512];
extern "C" u64 boot_pdpt[512];
extern "C" u64 boot_pd0[512];
extern "C" u64 boot_pd0_pts[512 * (MAX_KERNEL_SIZE >> 21 & 0x1ff)];
extern "C" u64 boot_pd_kernel[512];
extern "C" u64 boot_pd_kernel_pt0[512];
extern "C" u64 boot_pd_kernel_image_pts[512 * (MAX_KERNEL_SIZE >> 21 & 0x1ff)];
extern "C" u64 boot_pd_kernel_pt1023[512];
extern "C" char const kernel_cmdline[4096];

extern "C" void reload_cr3();

extern "C" {
multiboot_info_t* multiboot_info_ptr;
}

[[noreturn]] static void halt()
{
    asm volatile("hlt")
    __builtin_unreachable()
}

void __stack_chk_fail()
{
    halt();
}

void __assertion_failed(char const*, const char*, unsigned int, char const*)
{
    halt();
}

void* __dso_handle __attribute__((visibility("hidden")));
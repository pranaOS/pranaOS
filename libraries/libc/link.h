/**
 * @file link.h
 * @author Krisna Pranav
 * @brief link
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#ifndef __pranaos__
#    include <libc/elf.h>
#else
#    include <elf.h>
#endif
#include <limits.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

struct dl_phdr_info 
{
    ElfW(Addr) dlpi_addr;
    const char* dlpi_name;
    const ElfW(Phdr) * dlpi_phdr;
    ElfW(Half) dlpi_phnum;
}; // struct dl_phdr_info 

/**
 * @param callback 
 * @param data 
 * @return int 
 */
int dl_iterate_phdr(int (*callback)(struct dl_phdr_info* info, size_t size, void* data), void* data);

__END_DECLS

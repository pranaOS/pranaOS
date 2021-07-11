/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/Prelude.h>

namespace Arch::x86_64
{

struct PACKED PML4Entry
{
    bool present : 1;               
    bool writable : 1;              
    bool user : 1;                  
    bool write_thought : 1;         
    bool cache : 1;                 
    bool accessed : 1;              
    int zero0 : 6;                  
    uint64_t physical_address : 36; 
    int zero1 : 15;                
    bool execute_disabled : 1;     
};

struct PACKED PML4 : public AddressSpace
{
    PML4Entry entries[512];
};

static inline size_t pml4_index(uintptr_t address)
{
    return (address >> 39) & 0x1FF;
}

static_assert(sizeof(PML4Entry) == sizeof(uint64_t));
static_assert(sizeof(PML4) == 4096);

struct PACKED PML3Entry
{
    bool present : 1;               
    bool writable : 1;              
    bool user : 1;                  
    bool write_thought : 1;         
    bool cache : 1;                 
    bool accessed : 1;              
    int zero0 : 1;                  
    int size : 1;                   
    int zero1 : 4;                  
    uint64_t physical_address : 36; 
    int zero2 : 15;                 
    bool execute_disabled : 1;      
};

struct PACKED PML3
{
    PML3Entry entries[512];
};

static inline size_t pml3_index(uintptr_t address)
{
    return (address >> 30) & 0x1FF;
}

static_assert(sizeof(PML3Entry) == sizeof(uint64_t));
static_assert(sizeof(PML3) == 4096);

struct PACKED PML2Entry
{
    bool present : 1;               // Must be 1 to reference a PML-1
    bool writable : 1;              // If 0, writes may not be allowed.
    bool user : 1;                  // If 0, user-mode accesses are not allowed
    bool write_thought : 1;         // Page-level write-through
    bool cache : 1;                 // Page-level cache disable
    bool accessed : 1;              // Indicates whether this entry has been used
    int zero0 : 1;                  // Ignored
    int size : 1;                   // Must be 0 otherwise, this entry maps a 2-MByte page.
    int zero1 : 4;                  // Ignored
    uint64_t physical_address : 36; // Physical address of a 4-KByte aligned PLM-1
    int zero2 : 15;                 // Ignored
    bool execute_disabled : 1;      // If IA32_EFER.NXE = 1, Execute-disable
};

struct PACKED PML2
{
    PML3Entry entries[512];
};

static inline size_t pml2_index(uintptr_t address)
{
    return (address >> 21) & 0x1FF;
}

static_assert(sizeof(PML2Entry) == sizeof(uint64_t));
static_assert(sizeof(PML2) == 4096);

}
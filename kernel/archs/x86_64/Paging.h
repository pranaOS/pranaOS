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

struct PACKED PML1Entry
{
    bool present : 1;               
    bool writable : 1;              
    bool user : 1;                  
    bool write_thought : 1;         
    bool cache : 1;                
    bool accessed : 1;             
    int dirty : 1;                 
    int memory_type : 1;           
    int global : 1;                
    int zero0 : 3;                 
    uint64_t physical_address : 36;
    int zero1 : 10;                
    bool protection_key : 5;        
    bool execute_disabled : 1;      
};

struct PACKED PML1
{
    PML3Entry entries[512];
};

static inline size_t pml1_index(uintptr_t address)
{
    return (address >> 12) & 0x1FF;
}

static_assert(sizeof(PML1Entry) == sizeof(uint64_t));
static_assert(sizeof(PML1) == 4096);

extern "C" void paging_load_directory(uintptr_t directory);

extern "C" void paging_invalidate_tlb();

PML4 *kernel_pml4();

PML4 *pml4_create();

void pml4_destroy(PML4 *pml4);

void pml4_switch(PML4 *pml4);

void virtual_initialize();

void virtual_memory_enable();

bool virtual_present(PML4 *pml4, uintptr_t virtual_address);

uintptr_t virtual_to_physical(PML4 *pml4, uintptr_t virtual_address);

JResult virtual_map(PML4 *pml4, MemoryRange physical_range, uintptr_t virtual_address, MemoryFlags flags);

MemoryRange virtual_alloc(PML4 *pml4, MemoryRange physical_range, MemoryFlags flags);

void virtual_free(PML4 *pml4, MemoryRange virtual_range);

}
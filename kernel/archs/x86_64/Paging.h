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

}